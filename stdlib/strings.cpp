#include "../alisp.hpp"
#include "../stdlib.hpp"

#include "../contrib/TextUtils.h"

using namespace ALisp::Atoms;

namespace ALisp {
	namespace Stdlib {
		// string:split(Seperator, String) -> list()
		Cell strings_split(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			std::vector<StringType> result;
			StringType seperator = args[0].str();
			StringType str = args[1].str();
			ListCell retvalue;
			if (seperator == "") {
				// No seperator given, just split every character
				for (auto it = str.cbegin(); it != str.cend(); ++it)
					retvalue.push(StringCell(*it));
			} else {
				char csep = seperator[0];
				TextUtils::Split(csep, result, str);
				// Convert vector of strings into correct ALisp cell
				for (auto it = result.cbegin(); it != result.cend(); ++it)
					retvalue.push(StringCell(*it));
			}
			return retvalue;
		}

		// string:join(Seperator, list) -> string()
		Cell strings_join(const ListType &args) {
			RUNTIME_CHECK(args, runtime_check::has_two_arguments);
			StringType seperator = args[0].str();
			Cell items = args[1];
			auto b = items.cbegin(), e = items.cend();
			std::ostringstream s;
			// TODO: more efficiently
			for (auto it = b; it != e; ++it) {
				if (it != b)
					s << seperator;
				s << it->str();
			}
			return StringCell(s.str());
		}
	}
}