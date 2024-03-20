#pragma once

#include <string_view>
#include <vector>

#include "../global/defs.hpp"

namespace Nitro {

struct Token {
	enum class Type {
		// Enclosures and scoping
		OpenParen, CloseParen,
		OpenBracket, CloseBracket,
		Indent, Dedent,

		Eol,	// End of line

		// Mathematical symbols
		Plus, Minus, Star, StarStar, Slash,
		Greater, GreaterGreater, GreaterEqual, Less, LessLess, LessEqual,
		Equal, EqualEqual, Not, NotEqual,
		And, AndAnd, Pipe, PipePipe,
		Tilde, Carat,
		Colon, Question, Comma,

		// Literals
		FloatLiteral, IntegerLiteral, Identifier,
		CharLiteral, StringLiteral,

		// Keywords
		IfKeyword, ElseKeyword,
		WhileKeyword, ForKeyword, ContinueKeyword, BreakKeyword,
		ReturnKeyword, FuncKeyword,
		LetKeyword,
		TrueKeyword, FalseKeyword, NilKeyword,

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
	static constexpr std::size_t TAB_WIDTH = 4; // Spaces

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

	Token::Type checkKeyword(std::size_t start, std::size_t length, std::string_view rest, Token::Type type);
	Token::Type identifierOrKeywordType();

	Token identifierOrKeyword();
	Token characterLiteral();
	Token stringLiteral();
};
	
} // namespace Nitro
