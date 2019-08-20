// stdlib/file.cpp
// 
// Contains functions for operating on files

#include <fstream>
#include <sstream>
#include <string>

#include "../alisp.hpp"
#include "../stdlib.hpp"

namespace ALisp {
	namespace Stdlib {
		namespace detail {
			const char *search_paths[] = {
				"",
				"./",
				"./lisp/",
				0
			};
		}

		Cell file_exists(StringType file) {
			std::ifstream fs(file);
			return fs.good() ? True : False;
		}
		// (file:exists File) -> bool()
		// No search paths used.
		Cell file_exists(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			return file_exists(args[0].str());
		}

		// (file:path File) -> Path
		// where File is FileReference(),
		//       Path is FileReference();
		// and Path is subpath of search_paths().
		Cell file_path(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			for (auto it = &detail::search_paths[0]; *it != nullptr; ++it) {
				StringType path = *it;
				StringType full = path + args[0].str();
				if (file_exists(full) == True)
					return StringCell(full);
			}
			return StringCell(args[0].str());
		}

		Cell file_read(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			Cell path = file_path(args);
			std::ifstream fs(path.str());
			if (!fs.good())
				throw Exception("Failed to open file: " + path.str());
			std::stringstream buffer;
			buffer << fs.rdbuf();
			return StringCell(buffer.str());
		}
	}
}