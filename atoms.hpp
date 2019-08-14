// atoms.hpp
//
// Provides the handling of atoms.
// Atoms do not require high insertion times, as most atoms will be
// created at read time, not run time.
// As a result, a standard set of maps is used.
#pragma once

#include <map>

#include "acore.hpp"

namespace ALisp {
	namespace Atoms {
		typedef std::map<AtomType, StringType> AtomStringMap;
		typedef std::map<StringType, AtomType> StringAtomMap;
		typedef std::map<AtomType, Cell> AtomCellMap;

		namespace detail {
			extern AtomStringMap atom_to_string;
			extern StringAtomMap string_to_atom;
			extern AtomCellMap atom_to_cell;
			extern AtomType atom_counter;
		}

		std::string ToString(AtomType id) EXCEPT;
		bool Exists(AtomType id);
		bool Exists(const char *name);
		Cell Get(AtomType id) EXCEPT;
		Cell Get(const char *name) EXCEPT;
		Cell Declare(const char *name) EXCEPT;
	}
}