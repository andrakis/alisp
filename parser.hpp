#pragma once

#include <list>
#include <sstream>
#include <tuple>

#include "acore.hpp"
#include "cell.hpp"

namespace ALisp {
	namespace Parser {
		namespace detail {
			template<typename T>
			std::tuple<bool, T> tryStringToNumber(const std::string &str) {
				T value;
				std::stringstream stream(str);
				stream >> value;
				return std::make_tuple(!stream.fail(), value);
			}

			typedef StringType TokenType;
			typedef std::list<TokenType> TokenListType;
			TokenListType tokenise(const TokenType &str) EXCEPT;
			Cell atom(const TokenType &token) EXCEPT;
			Cell read_from(TokenListType &tokens) EXCEPT;
			Cell read(const StringType &str) EXCEPT;
		}

		Cell read(const StringType &str);
	}
}
