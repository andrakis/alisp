#include "acore.hpp"
#include "cell.hpp"

#include "atoms.hpp"

namespace ALisp {
	Cell Nil(CellType::Atom, 0);
	Cell True(CellType::Atom, 1);
	Cell False(CellType::Atom, 2);

	StringType Cell::str() const {
		return Atoms::ToString(atomId());
	}

	std::ostream &operator<<(std::ostream &os, const Cell &cell) {
		os << cell.str();
		return os;
	}
}
