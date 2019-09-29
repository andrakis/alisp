/** 
 * Stdlib - Environment functions
 */

#include <iostream>

#include "../alisp.hpp"
#include "../stdlib.hpp"

namespace ALisp {
	namespace Stdlib {
		Cell env_defined(const ListType &args, EnvironmentType env) {
			RUNTIME_CHECK(args, runtime_check::has_atleast_one_argument);
			return env->has_key_any(args[0].atomId()) ? True : False;
		}
	}
}
