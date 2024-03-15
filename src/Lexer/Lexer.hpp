#pragma once

#include <string_view>

#include "../global/defs.hpp"

namespace Nitro {

struct Token {
	enum class Type {
		// Enclosures and scoping
		OpenParen, CloseParen,
		Indent, Dedent,

		Eol,	// End of line

		Identifier,

		// Mathematical symbols
		Plus, Minus, Star, StarStar, Slash,

		// Literals
		FloatLiteral, IntegerLiteral,

		// Other
	        Eof,
		Error
	} type;

	std::string_view lexeme;

	std::size_t line;
	std::size_t col;
};

class Lexer {
public:
	NITRO_DISABLE_COPY_MOVE(Lexer)

	explicit Lexer(std::string_view source);

	Token next();

private:
	unsigned m_dedent_emit_count = 0;
	std::string_view m_source;
	std::size_t m_current;
	std::size_t m_start;
	std::size_t m_line;
	std::size_t m_col;

	// lexing helpers
	char advance();
	char peek();
	bool match(char expected);

	Token simple(Token::Type type);
	Token endOfLine();
	Token number();
	Token error(std::string_view msg);
	Token identifierOrKeyword();
};
	
}; // namespace Nitro
