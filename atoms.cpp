#include "atoms.hpp"
#include "cell.hpp"

namespace ALisp {
	namespace Atoms {
		namespace detail {
			AtomStringMap atom_to_string;
			StringAtomMap string_to_atom;
			AtomCellMap atom_to_cell;
			AtomType atom_counter = 0;
		}

		std::string ToString(AtomType id) EXCEPT {
			for (auto it = detail::atom_to_string.cbegin(); it != detail::atom_to_string.cend(); ++it)
				if (it->first == id)
					return it->second;
			throw new Exception("atomid " + std::to_string(id) + " not found");
		}
		bool Exists(AtomType id) {
			return detail::atom_to_string.find(id) != detail::atom_to_string.end();
		}
		bool Exists(const char *name) {
			return detail::string_to_atom.find(StringType(name)) != detail::string_to_atom.end();
		}

		Cell Get(AtomType id) EXCEPT {
			auto it = detail::atom_to_cell.find(id);
			if (it == detail::atom_to_cell.end())
				throw new Exception("atomid " + std::to_string(id) + " not found");
			return it->second;
		}
		Cell Get(const char *name) EXCEPT {
			auto it = detail::string_to_atom.find(StringType(name));
			if (it == detail::string_to_atom.end())
				throw new Exception("atom " + StringType(name) + " not found");
			return Get(it->second);
		}
		Cell Declare(const char *name) EXCEPT {
			return Declare(name, ++detail::atom_counter);
		}
		Cell Declare(const char *name, AtomType id) EXCEPT {
			if (Exists(name))
				return Get(name);
			Cell atom(CellType::Atom, id);
			detail::atom_to_cell.emplace(id, atom);
			detail::atom_to_string.emplace(id, StringType(name));
			detail::string_to_atom.emplace(StringType(name), id);
			return atom;
		}
	}
}