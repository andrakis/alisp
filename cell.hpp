#pragma once

#include <memory>
#include <ostream>
#include <type_traits>
#include <string>
#include <sstream>

#include "acore.hpp"
#include "atoms.hpp"

namespace ALisp {
	enum CellType {
		None,
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

	struct Cell; // forward dec
	extern Cell Nil, True, False;

	struct Cell {
	public:
		typedef ListType::const_iterator const_iterator;

		// Cell interface
		struct ICell {
			ICell(CellType type) : _type(type) {}
			virtual std::unique_ptr<ICell> clone() const = 0;
			virtual ~ICell() {}

			CellType type() const { return _type; }
			void type(CellType t) { _type = t; }

			// Type accessors
			virtual AtomType atomId() const EXCEPT { throw Exception("Not an atom"); }
			virtual IntegerType integerValue() const EXCEPT { throw Exception("Not an integer"); }
			virtual FloatType floatValue() const EXCEPT { throw Exception("Not a float"); }
			virtual StringType str() const { return "TODO"; }
			virtual EnvironmentType env() EXCEPT { throw Exception("Not an environment"); }
			virtual ProcType proc() const EXCEPT { throw Exception("Not a proc"); }
			virtual ProcEnvType proc_env() const EXCEPT { throw Exception("Not a proc_env"); }

			virtual void multiply(const Cell &other) EXCEPT { throw Exception("Cannot multiply"); }
			virtual void add(const Cell &other) EXCEPT { throw Exception("Cannot add"); }
			virtual void subtract(const Cell &other) EXCEPT { throw Exception("Cannot subtract"); }
			virtual void divide(const Cell &other) EXCEPT { throw Exception("Cannot divide"); }

			// List functions
			virtual void push(const Cell &value) EXCEPT { throw Exception("Not a list"); }
			virtual Cell head() const EXCEPT { throw Exception("Not a list"); }
			virtual Cell tail() const EXCEPT { throw Exception("Not a list"); }
			virtual bool empty() const EXCEPT { throw Exception("Not a list"); }
			virtual size_t size() const EXCEPT { throw Exception("Not a list"); }
			virtual Cell index(size_t i) const EXCEPT { throw Exception("Not a list"); }
			virtual const_iterator cbegin() const EXCEPT { throw Exception("Not a list"); }
			virtual const_iterator cend() const EXCEPT { throw Exception("Not a list"); }

			// Lambda/macro functions
			virtual Cell lambda_ptr() const EXCEPT { throw Exception("Not a lambda-like"); }
			virtual void lambda_ptr(const Cell &ptr) EXCEPT { throw Exception("Not a lambda-like"); }
			virtual Cell lambda_args() const EXCEPT { throw Exception("Not a lambda-like"); }
			virtual void lambda_args(const Cell &ptr) EXCEPT { throw Exception("Not a lambda-like"); }
			virtual Cell lambda_body() const EXCEPT { throw Exception("Not a lambda-like"); }
			virtual void lambda_body(const Cell &ptr) EXCEPT { throw Exception("Not a lambda-like"); }
		protected:
			CellType _type;
		};
	public:
		Cell(Cell&&)=default;
		Cell& operator=(Cell&&)=default;
		Cell(Cell const &o) : Cell(o.pImpl ? Cell(o.pImpl->clone()) : Cell()) {}
		Cell()=default;
		Cell& operator=(Cell const &o) {
			Cell tmp(o);
			swap(pImpl, tmp.pImpl);
			return *this;
		}
		Cell(std::unique_ptr<ICell> x) : pImpl(std::move(x)) {}

		CellType type() const {
			if (!pImpl) return None;
			return pImpl->type();
		}

		AtomType atomId() const EXCEPT {
			if (!pImpl) return 0;
			return pImpl->atomId();
		}
		IntegerType integerValue() const EXCEPT {
			if (!pImpl) return 0;
			return pImpl->integerValue();
		}
		FloatType floatValue() const EXCEPT {
			if (!pImpl) return 0;
			return pImpl->floatValue();
		}
		StringType str() const {
			if (!pImpl) return "";
			return pImpl->str();
		}
		EnvironmentType env() const EXCEPT {
			if (!pImpl) return EnvironmentType(nullptr);
			return pImpl->env();
		}

		Cell& operator+= (const Cell &other) EXCEPT {
			if (pImpl) {
				pImpl->add(other);
			}
			return *this;
		}
		Cell& operator-= (const Cell &other) EXCEPT {
			if (pImpl) {
				pImpl->subtract(other);
			}
			return *this;
		}
		Cell& operator*= (const Cell &other) EXCEPT {
			if (pImpl) {
				pImpl->multiply(other);
			}
			return *this;
		}
		Cell& operator/= (const Cell &other) EXCEPT {
			if (pImpl) {
				pImpl->divide(other);
			}
			return *this;
		}

		// List functions
		Cell head() const EXCEPT;
		Cell tail() const EXCEPT;
		bool empty() const EXCEPT;
		size_t size() const EXCEPT;
		void push(const Cell &value) EXCEPT { if (pImpl) pImpl->push(value); }
		Cell index(size_t i) const EXCEPT;
		const_iterator cbegin() const EXCEPT {
			if (pImpl) return pImpl->cbegin();
			throw Exception("Not a list");
		}
		const_iterator cend() const EXCEPT {
			if (pImpl) return pImpl->cend();
			throw Exception("Not a list");
		}

		// Lambda/Macro functions
		Cell lambda_ptr() const EXCEPT;
		void lambda_ptr(const Cell &) EXCEPT;
		Cell lambda_args() const EXCEPT;
		void lambda_args(const Cell &) EXCEPT;
		Cell lambda_body() const EXCEPT;
		void lambda_body(const Cell &) EXCEPT;

		// Proc and ProcEnv
		ProcType proc() const EXCEPT {
			if (pImpl) return pImpl->proc();
			throw Exception("Cell: Not a proc()");
		}
		ProcEnvType proc_env() const EXCEPT {
			if (pImpl) return pImpl->proc_env();
			throw Exception("Cell: Not a proc_env()");
		}
	protected:
		std::unique_ptr<ICell> pImpl;
	};

	template<CellType CellTypeType = CellType::None>
	struct NoneTypeCell : Cell {
		struct INoneTypeCell : ICell {
			INoneTypeCell() : ICell(CellTypeType) {}
			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<INoneTypeCell>();
			}

			StringType str() const final override { return "(none)"; }
		};
		NoneTypeCell()
			: Cell(std::make_unique<INoneTypeCell>()) {}
	};

	template<typename ValueType = AtomType, CellType CellTypeType = CellType::Atom>
	struct AtomTypeCell : Cell {
		struct IAtomCell : ICell {
			IAtomCell(ValueType value) :
				ICell(CellTypeType),
				_value(value) {}

			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IAtomCell>(_value);
			}

			AtomType atomId() const EXCEPT final override { return _value; }

			StringType str() const final override { return Atoms::ToString(_value); }
		protected:
			ValueType _value;
		};

		AtomTypeCell(ValueType value)
			: Cell(std::make_unique<IAtomCell>(value)) {}
	};

	template<typename ValueType = StringType, CellType CellTypeType = CellType::String>
	struct StringTypeCell : Cell {
		struct IStringCell : ICell {
			IStringCell(ValueType value) :
				ICell(CellTypeType),
				_value(value) {}

			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IStringCell>(_value);
			}

			StringType str() const final override { return _value; }

			void add(const Cell &other) EXCEPT final override { _value += other.str(); }
		protected:
			ValueType _value;
		};

		StringTypeCell(ValueType value)
			: Cell(std::make_unique<IStringCell>(value)) {}
	};

	template<typename ValueType, CellType CellTypeType>
	struct MathematicalCell : Cell {
		struct IMathematicalCell : ICell {
			IMathematicalCell(ValueType value) :
				ICell(CellTypeType),
				_value(value) {}

			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IMathematicalCell>(_value);
			}

			IntegerType integerValue() const EXCEPT final override {
				return (IntegerType)_value;
			}

			FloatType floatValue() const EXCEPT final override {
				return (FloatType)_value;
			}

			StringType str() const final override { return std::to_string(_value); }

			void add(const Cell &other) EXCEPT final override { _value += convertOther(other); }
			void subtract(const Cell &other) EXCEPT final override { _value -= convertOther(other); }
			void multiply(const Cell &other) EXCEPT final override { _value *= convertOther(other); }
			void divide(const Cell &other) EXCEPT final override { _value /= convertOther(other); }
		protected:
			ValueType _value;
			ValueType convertOther(const Cell &other) const EXCEPT {
				switch (other.type()) {
					case CellType::Integer: return (ValueType)other.integerValue();
					case CellType::Float: return (ValueType)other.floatValue();
					default:
						throw Exception("Unable to convert other");
				}
			}
		};

		MathematicalCell(ValueType value)
			: Cell(std::make_unique<IMathematicalCell>(value)) {}
	};

	template<typename ValueType = ListType, CellType CellTypeType = CellType::List>
	struct ListTypeCell : Cell {
		typedef typename ValueType::iterator iterator;
		typedef typename ValueType::const_iterator const_iterator;
	
		struct IListTypeCell;

		ListTypeCell() : Cell(std::make_unique<IListTypeCell>()) {}
		ListTypeCell(const ValueType &other) : Cell(std::make_unique<IListTypeCell>(other)) {}
		template<typename InputIterator> ListTypeCell(InputIterator first, InputIterator last)
			: Cell(std::make_unique<IListTypeCell>(first, last)) {}

		struct IListTypeCell : ICell {
			IListTypeCell() : ICell(CellTypeType), _value() {}
			IListTypeCell(const ValueType &other) : ICell(CellTypeType), _value(other) {}
			template<typename InputIterator> IListTypeCell(InputIterator first, InputIterator last) : ICell(CellTypeType), _value(first, last) {}

			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IListTypeCell>(_value.cbegin(), _value.cend());
			}

			StringType str() const final override {
				std::ostringstream ss;
				ss << stringOpen<CellTypeType>();
				for (auto it = _value.cbegin(); it != _value.cend(); ++it) {
					if (it != _value.cbegin())
						ss << " ";
					ss << it->str();
				}
				ss << stringClose<CellTypeType>();
				return ss.str();
			}

			void push(const Cell &value) EXCEPT final override { _value.push_back(value); }
			Cell head() const EXCEPT final override {
				if (_value.empty()) return Nil;
				return *_value.cbegin();
			}
			Cell tail() const EXCEPT final override {
				return ListTypeCell(_value.cbegin() + 1, _value.cend());
			}
			bool empty() const EXCEPT final override { return _value.empty(); }
			size_t size() const EXCEPT final override { return _value.size(); }
			const_iterator cbegin() const EXCEPT final override { return _value.cbegin(); }
			const_iterator cend() const EXCEPT final override { return _value.cend(); }

			// Lambda/macro functions
			Cell lambda_ptr() const EXCEPT final override { return _value[lm_ptr]; }
			void lambda_ptr(const Cell &ptr) EXCEPT final override { _value[lm_ptr] = ptr; }
			Cell lambda_args() const EXCEPT final override { return _value[lm_args]; }
			void lambda_args(const Cell &args) EXCEPT final override { _value[lm_args] = args; }
			Cell lambda_body() const EXCEPT final override { return _value[lm_body]; }
			void lambda_body(const Cell &body) EXCEPT final override { _value[lm_body] = body; }
		protected:
			ValueType _value;
			template<CellType> StringType stringOpen() const { return "("; }
			template<> StringType stringOpen<CellType::Lambda>() const { return "(#Lambda "; }
			template<> StringType stringOpen<CellType::Macro>() const { return "(#Macro "; }

			template<CellType> StringType stringClose() const { return ")"; }

			static const size_t lm_ptr = 0, lm_args = 1, lm_body = 2;
		};
	};

	template<typename ProcTypeType = ProcType, CellType CellTypeType = CellType::Proc>
	struct ProcTypeCell : Cell {
		struct IProcTypeCell : ICell {
			IProcTypeCell(ProcTypeType proc) : ICell(CellTypeType), _proc(proc) {}

			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IProcTypeCell>(_proc);
			}

			StringType str() const final override { return "(#Proc)"; }

			ProcType proc() const EXCEPT final override { return _proc; }
			ProcEnvType proc_env() const EXCEPT final override { throw Exception("Not a ProcEnv"); }
		protected:
			ProcTypeType _proc;
		};

		ProcTypeCell(ProcTypeType proc) : Cell(std::make_unique<IProcTypeCell>(proc)) {}
	};

	template<typename ProcTypeType = ProcEnvType, CellType CellTypeType = CellType::ProcEnv>
	struct ProcEnvTypeCell : Cell {
		struct IProcEnvTypeCell : ICell {
			IProcEnvTypeCell(ProcTypeType proc) : ICell(CellTypeType), _proc(proc) {}

			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IProcEnvTypeCell>(_proc);
			}

			StringType str() const final override { return "(#ProcEnv)"; }

			ProcType proc() const EXCEPT final override { throw Exception("Not a Proc"); }
			ProcEnvType proc_env() const EXCEPT final override { return _proc; }
		protected:
			ProcTypeType _proc;
		};

		ProcEnvTypeCell(ProcTypeType proc) : Cell(std::make_unique<IProcEnvTypeCell>(proc)) {}
	};

	template<CellType CellTypeType>
	struct EnvironmentTypeCell : Cell {
		struct IEnvironmentTypeCell : ICell {
			IEnvironmentTypeCell(EnvironmentType value) : ICell(CellTypeType), _env(value) {}
			std::unique_ptr<ICell> clone() const final override {
				return std::make_unique<IEnvironmentTypeCell>(_env);
			}

			EnvironmentType env() EXCEPT final override { return _env; }

			StringType str() const final override { return "(#env)"; }
		protected:
			EnvironmentType _env;
		};
		EnvironmentTypeCell(EnvironmentType env) : Cell(std::make_unique<IEnvironmentTypeCell>(env)) {}
	};

	using NoneCell = NoneTypeCell<CellType::None>;
	using AtomCell = AtomTypeCell<AtomType, CellType::Atom>;
	using StringCell = StringTypeCell<StringType, CellType::String>;
	using IntCell = MathematicalCell<IntegerType, CellType::Integer>;
	using FloatCell = MathematicalCell<FloatType, CellType::Float>;
	using ListCell = ListTypeCell<ListType, CellType::List>;
	using LambdaCell = ListTypeCell<ListType, CellType::Lambda>;
	using MacroCell = ListTypeCell<ListType, CellType::Macro>;
	using ProcCell = ProcTypeCell<ProcType, CellType::Proc>;
	using ProcEnvCell = ProcEnvTypeCell<ProcEnvType, CellType::ProcEnv>;
	using EnvironmentCell = EnvironmentTypeCell<CellType::EnvPointer>;

	std::ostream &operator<<(std::ostream &os, const Cell &cell);
}
