#pragma once

#include <list>

#include "acore.hpp"
#include "excepts.hpp"
#include "cell.hpp"

namespace ALisp {
	namespace Parser {
		namespace detail {
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
