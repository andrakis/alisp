#include <iostream>
#include "../alisp.hpp"

namespace ALisp {
	namespace Stdlib {
		Cell print(const ListType &args) {
			for (auto it = args.cbegin(); it != args.cend(); ++it) {
				if (it != args.cbegin())
					std::cout << " ";
				std::cout << it->str();
			}
			std::cout << std::endl;
			return Nil;
		}
	}
}
