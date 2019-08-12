#include <ctype.h>

#include "parser.hpp"

namespace ALisp {
	namespace Parser {
		namespace detail {
			char startOf(const TokenType &str) { return str[0]; }
			char endOf(const TokenType &str) { return *(str.cend() - 1); }
			TokenListType tokenise(const TokenType &str) EXCEPT {
				TokenListType tokens;
				const char *s = str.c_str();
				while (*s) {
					// Skip whitespace
					while (isspace(*s))
						++s;
					// Skip comment lines
					if (*s == ';' && *(s + 1) == ';')
						while (*s && *s != '\n' && *s != '\r')
							++s;
					// List open or close
					else if (*s == '(' || *s == ')')
						tokens.push_back(*s++ == '(' ? "(" : ")");
					// "String" in quotes
					else if (*s == '"') {
						const char *t = s;
						int escape = 0;
						do {
							++t;
							if (escape != 0) escape--;
							if (*t == '\\')
								escape = 2; // skip this and the next character
						} while (*t && (escape != 0 || *t != '"'));
						++t;
						tokens.push_back(StringType(s, t));
						s = t;
					// A generic token
					} else {
						const char *t = s;
						while (*t && !isspace(*t) && *t != '(' && *t != ')')
							++t;
						tokens.push_back(StringType(s, t));
						s = t;
					}
				}
				return tokens;
			}
			Cell atom(const TokenType &token) EXCEPT {
				if (startOf(token) == '"' && endOf(token) == '"')
					return StringCell(token.substr(1, token.length() - 2));
				if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
					// Number
					auto isInteger = tryStringToNumber<IntegerType>(token);
					if (std::get<0>(isInteger))
						return IntCell(std::get<1>(isInteger));
					auto isFloat = tryStringToNumber<FloatType>(token);
					if (std::get<0>(isFloat))
						return FloatCell(std::get<1>(isFloat));
				}
				return Atoms::Declare(token.c_str());
			}
			Cell read_from(TokenListType &tokens) EXCEPT {
				const TokenType token(tokens.front());
				tokens.pop_front();
				if (token == "(") {
					ListCell cells;
					while (tokens.front() != ")") {
						cells.push(read_from(tokens));
					}
					tokens.pop_front();
					return cells;
				} else
					return atom(token);
			}
			Cell read(const StringType &str) EXCEPT {
				TokenListType tokens(tokenise(str));
				return read_from(tokens);
			}
		}
		Cell read(const StringType &str) {
			return detail::read(str);
		}
	}
}