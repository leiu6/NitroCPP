#pragma once

#include <string_view>
#include <vector>

#include "../global/defs.hpp"

namespace Nitro {

struct Token {
	enum class Type {
		// Enclosures and scoping
		OpenParen, CloseParen,
		Indent, Dedent,

		Eol,	// End of line

		// Mathematical symbols
		Plus, Minus, Star, StarStar, Slash,

		// Literals
		FloatLiteral, IntegerLiteral, Identifier,

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
	bool m_line_begin = true;
	std::vector<unsigned> m_indent_levels = { 0 };
	unsigned m_dedent_emit_count = 0;
	std::string_view m_source;
	std::size_t m_current;
	std::size_t m_start;
	std::size_t m_line;
	std::size_t m_col;

	// lexing helpers
	
	/**
	* Skips all spaces. Does not skip tabs, since those control indent and
	* dedent.
	*/
	char getFirstNonWhitespace();
	char advance();
	char peek();
	bool match(char expected);

	Token simple(Token::Type type);
	Token endOfLine();
	unsigned getCurrentIndentLevel();
	unsigned determineLevelsOfDedent(unsigned current);
	bool indentDedent(Token& token_out);
	Token number();
	Token error(std::string_view msg);
	Token identifierOrKeyword();
};
	
}; // namespace Nitro
