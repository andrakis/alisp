#pragma once

#include <ostream>
#include <type_traits>

#include "acore.hpp"

namespace ALisp {
	enum CellType {
		Atom,
		Integer,
		Float,
		String,
		List,
		Lambda,
		Macro,
		Proc,
		ProcEnv,
		EnvPointer
	};

	// Basic Cell class
	class Cell {
	protected:
		void assign(const Cell& a) {
			this->_atomId = a.atomId();
		}
	public:
		Cell(CellType CellTypeType = CellType::Atom, AtomType atomId = 0)
			: _type(CellTypeType), _atomId(atomId) {}
		virtual ~Cell() {}

		CellType type() const { return _type; }

		virtual AtomType atomId() const EXCEPT { return _atomId; }
		virtual IntegerType integerValue() const EXCEPT { throw new Exception("cannot convert to integer"); }
		virtual FloatType floatValue() const EXCEPT { throw new Exception("cannot convert to float"); }
		virtual StringType str() const;

		virtual Cell add(const Cell &other) const EXCEPT { throw new Exception("Cannot add"); }
		Cell operator+ (const Cell &other) const EXCEPT { return add(other); }
		virtual Cell subtract(const Cell &other) const EXCEPT { throw new Exception("Cannot subtract"); }
		Cell operator- (const Cell &other) const EXCEPT { return subtract(other); }
		virtual Cell multiply(const Cell &other) const EXCEPT { throw new Exception("Cannot multiply"); }
		Cell operator* (const Cell &other) const EXCEPT { return multiply(other); }
		virtual Cell divide(const Cell &other) const EXCEPT { throw new Exception("Cannot divide"); }
		Cell operator/ (const Cell &other) const EXCEPT { return divide(other); }

		// List functions
		virtual Cell head() const { throw new Exception("Cannot get head"); }
		virtual Cell tail() const { throw new Exception("Cannot get tail"); }

		// Lambda/Macro functions
		virtual EnvironmentType envptr() const { throw new Exception("Cannot get envptr()"); }
	protected:
	private:
		CellType _type;
		AtomType _atomId;
	};

	extern Cell Nil, True, False;

	template<typename ValueType, CellType CellTypeType>
	class SimpleCell : public Cell {
	protected:
		ValueType _value;
		void assign(const SimpleCell<ValueType, CellTypeType> &b) {
			Cell::assign(b);
			_value = b._value;
		}
	public:
		template<typename VT = ValueType,
			class = std::enable_if_t<std::is_default_constructible<std::remove_reference_t<VT>>::value>,
			class = void>
		SimpleCell() : Cell(CellTypeType), _value() {}
		template<typename VT = ValueType,
			class = std::enable_if_t<std::is_arithmetic<VT>::value>,
			class = void>
		SimpleCell(VT v) : Cell(CellTypeType), _value(v) {}
		template<typename VT = ValueType,
			class = std::enable_if_t<std::is_trivially_copyable<std::remove_reference_t<VT>>::value>,
			class = void>
		SimpleCell(const VT&&v) : Cell(CellTypeType), _value(v) {}

		// List type constructor
		template<typename VT = ValueType,
			class = std::enable_if_t<std::is_same<VT, ListType>::value>,
			class = void>
		SimpleCell(typename VT::const_iterator start, typename VT::const_iterator end)
			: Cell(CellTypeType), _value(start, end) {}

		virtual SimpleCell<ValueType,CellTypeType> &operator= (const Cell& a) {
			if (const SimpleCell<ValueType, CellTypeType>* b = dynamic_cast<const SimpleCell<ValueType, CellTypeType>*>(&a))
				assign(*b);
			throw new Exception("bad assignment");
			return *this;
		}
	};

	template<typename ValueType, CellType CellTypeType,
		class = std::enable_if_t<std::is_arithmetic<ValueType>::value>>
	class MathematicalCell : public SimpleCell<ValueType, CellTypeType> {
	public:
		ValueType value() const { return this->_value; }
		MathematicalCell(ValueType value = 0) : SimpleCell<ValueType, CellTypeType>(value) { }

		AtomType atomId() const EXCEPT { throw new Exception("Not an atom"); }
		IntegerType integerValue() const EXCEPT { return (IntegerType)this->_value; }
		FloatType floatValue() const EXCEPT { return (FloatType)this->_value; }
		StringType str() const EXCEPT { return std::to_string(this->_value); }

		Cell add(const Cell &other) EXCEPT { return MathematicalCell(this->_value + convertOther<ValueType>(other)); }
		Cell subtract(const Cell &other) EXCEPT { return MathematicalCell(this->_value - convertOther<ValueType>(other)); }
		Cell multiply(const Cell &other) EXCEPT { return MathematicalCell(this->_value * convertOther<ValueType>(other)); }
		Cell divide(const Cell &other) EXCEPT { return MathematicalCell(this->_value / convertOther<ValueType>(other)); }
	protected:
		template<typename To> To convertOther(const Cell &other) const EXCEPT {
			switch (other.type()) {
				case CellType::Integer: return (To)other.integerValue();
				case CellType::Float: return (To)other.floatValue();
				default:
					throw new Exception("Unable to convert other");
			}
		}
	};

	template<typename ValueType, CellType CellTypeType>
	class SpecificCell : public SimpleCell<ValueType, CellTypeType> {
	public:
		SpecificCell() : SimpleCell<ValueType, CellTypeType>() {}
		SpecificCell(ValueType &&other) : SimpleCell<ValueType, CellTypeType>(other) {}

		// List type constructor
		template<typename VT = ValueType,
			class = std::enable_if_t<std::is_same<VT, ListType>::value>,
			class = void>
		SpecificCell(typename VT::const_iterator start, typename VT::const_iterator end)
			: SimpleCell<ValueType, CellTypeType>(start, end) {}

		// Standard functions
		AtomType atomId() const EXCEPT { throw new Exception("Not an atom"); }

		// List functions
		template<typename VT = ValueType> std::enable_if_t<std::is_same<VT, ListType>::value, Cell> head() const {
			auto it = this->_value.cbegin();
			if (it != this->_value.end())
				return *it;
			return Nil;
		}
		template<typename VT = ValueType> std::enable_if_t<std::is_same<VT, ListType>::value, Cell> tail() const {
			auto it = this->_value.cbegin() + 1;
			return SpecificCell<VT, CellTypeType>(it, this->_value.end());
		}
	};

	template<typename ValueType, typename ExtraValueType, CellType CellTypeType>
	class ExtendedCell : public SpecificCell<ValueType, CellTypeType> {
		private:
		protected:
			ExtraValueType _extra;
		public:
			ExtendedCell(ValueType vt, ExtraValueType evt) : SpecificCell<ValueType, CellTypeType>(vt), _extra(evt) {}

			template<typename EVT = ExtraValueType>
			std::enable_if_t<std::is_same<EVT, EnvironmentType>::value, EnvironmentType> envptr() const {
				return _extra;
			}
	};

	using IntegerCell = MathematicalCell<long, CellType::Integer>;
	using FloatCell = MathematicalCell<double, CellType::Float>;
	using ListCell = SpecificCell<ListType, CellType::List>;
	using ProcCell = SpecificCell<ProcType, CellType::Proc>;
	using ProcEnvCell = SpecificCell<ProcEnvType, CellType::ProcEnv>;
	using LambdaCell = ExtendedCell<ListType, EnvironmentType, CellType::Lambda>;
	using MacroCell = ExtendedCell<ListType, EnvironmentType, CellType::Macro>;

	std::ostream &operator<<(std::ostream &os, const Cell &cell);
}
