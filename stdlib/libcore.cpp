// stdlib/libcore.cpp
// 
// Contains core functions that don't belong in a module.

#include <fstream>
#include <sstream>
#include <string>

#include "../alisp.hpp"
#include "../stdlib.hpp"

namespace ALisp {
	namespace Stdlib {
		// (head List) -> Element()
		//   where Element = List[0]
		Cell core_head(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			return args[0].head();
		}
		// (tail List) -> SubList() | Nil
		//   where [discard, SubList] = List
		Cell core_tail(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			return args[0].tail();
		}
		// (size List) -> integer()
		Cell core_size(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			return IntCell(args[0].size());
		}
		// (size_atleast List N) -> bool()
		Cell core_size_atleast(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0].size_atleast(args[1].integerValue()) ? True : False;
		}
		// (index List N) -> List[, N ...]
		Cell core_index(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			return args[0].index(args[1].integerValue());
		}
		// (list ...) -> list()
		Cell core_list(const ListType &args) {
			return ListCell(args);
		}
		// (none) -> none()
		Cell core_none(const ListType &args) {
			return NoneCell();
		}
		// (atom Name) -> atom()
		Cell core_atom(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_one_argument);
			return Atoms::Declare(args[0].str().c_str());
		}
		// (empty? List) -> bool()
		Cell core_empty(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_one_argument);
			return args[0].empty() ? True : False;
		}
	}
}
