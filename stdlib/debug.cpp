/** 
 * Stdlib - Debug functions
 */

#include <iostream>

#include "../alisp.hpp"

namespace ALisp {
	namespace Stdlib {
		size_t depthCount(size_t n, EnvironmentType env) {
			if (env->has_parent())
				return depthCount(n + 1, env->get_parent());
			return n;
		}
		size_t depthCount(EnvironmentType env) {
			return depthCount(1, env);
		}
		Cell print_env(const ListType &args, EnvironmentType env) {
			// Get depth count
			size_t depth = depthCount(env);
			auto e = env;
			for (;;) {
				for (auto it = e->cbegin(); it != e->cend(); ++it) {
					std::cout << std::string(depth, '-') << "> ";
					std::cout << Atoms::Get(it->first) << " = " << it->second << std::endl;
				}
				if (!e->has_parent())
					break;
				e = e->get_parent();
				--depth;
			}
			return Nil;
		}
	}
}