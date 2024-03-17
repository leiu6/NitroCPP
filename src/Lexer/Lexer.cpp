#include "Lexer.hpp"

#include <cctype>

namespace Nitro {

Lexer::Lexer(std::string_view source)
	: m_source(source), m_current(0), m_start(0), m_line(1), m_col(0) {}

char Lexer::advance() {
	// Since string view does not include the null byte by design, we will
	// artificially emit it so the lexer can stop.
	char c = (m_current < m_source.size()) ? m_source[m_current++] : '\0';

	m_col++;

	return c;
}

char Lexer::getFirstNonWhitespace() {
	char c;

	do {
		m_start = m_current;
		c = advance();
	} while (c == ' ' || c == '\t');

	return c;
}

char Lexer::peek() {
	return (m_current < m_source.size()) ? m_source[m_current] : '\0';
}

bool Lexer::match(char expected) {
	if (peek() == expected) {
		advance();
		return true;
	}
	return false;
}

Token Lexer::simple(Token::Type type) {
	return Token{
		type,
		m_source.substr(m_start, m_current - m_start),
		m_line,
		m_col	
	};
}

Token Lexer::endOfLine() {
	Token token{
		Token::Type::Eol,
		m_source.substr(m_start, m_current - m_start),
		m_line,
		m_col
	};
	m_line++;
	m_col = 0;
	m_line_begin = true;
	return token;
}

unsigned Lexer::getCurrentIndentLevel() {
	unsigned current = 0;

	char c;
	while ((c = peek()) == '\t') {
		m_col += TAB_WIDTH - 1;
		advance();
		current++;
	}
	return current;
}

unsigned Lexer::determineLevelsOfDedent(unsigned current) {
	unsigned dedents = 0;
	while (current < m_indent_levels.back()) {
		m_indent_levels.pop_back();
		dedents++;
	}
	return dedents;
}

bool Lexer::indentDedent(Token& token_out) {
	unsigned current = getCurrentIndentLevel();

	if (current > m_indent_levels.back()) {
		m_indent_levels.push_back(current);
		token_out = simple(Token::Type::Indent);
		return true;
	} else if (current < m_indent_levels.back()) {
		m_dedent_emit_count = determineLevelsOfDedent(current) - 1;
		token_out = simple(Token::Type::Dedent);
		return true;
	}
	else {
		return false;
	}
}

Token Lexer::number() {
	std::size_t line = m_line;
	std::size_t col = m_col;

	while (std::isdigit(peek())) {
		advance();		
	}

	if (!match('.')) {
		return Token{
			Token::Type::IntegerLiteral,
			m_source.substr(m_start, m_current - m_start),
			line,
			col
		};
	}

	while (std::isdigit(peek())) {
		advance();	
	}

	return Token{
		Token::Type::FloatLiteral,
		m_source.substr(m_start, m_current - m_start),
		line,
		col
	};
}

Token Lexer::error(std::string_view msg) {
	return Token{
		Token::Type::Error,
		msg,
		m_line,
		m_col
	};
}

Token::Type Lexer::checkKeyword(std::size_t start, std::size_t length, std::string_view rest, Token::Type type) {
	if (m_current - m_start == start + length &&
	    m_source.substr(m_start + start, length) == rest) {
		return type;
	}

	return Token::Type::Identifier;
}

Token::Type Lexer::identifierOrKeywordType() {
	switch (m_source[m_start]) {
		case 'b': return checkKeyword(1, 4, "reak", Token::Type::BreakKeyword);
		case 'c': return checkKeyword(1, 7, "ontinue", Token::Type::ContinueKeyword);
		case 'e': return checkKeyword(1, 3, "lse", Token::Type::ElseKeyword);
		case 'f': {
			if (m_current - m_start > 1) {
				switch (m_source[m_start + 1]) {
					case 'a': return checkKeyword(2, 3, "lse", Token::Type::FalseKeyword);
					case 'u': return checkKeyword(2, 2, "nc", Token::Type::FuncKeyword);
					case 'o': return checkKeyword(2, 1, "r", Token::Type::ForKeyword);
				}
			}
		} break;
		case 'i': return checkKeyword(1, 1, "f", Token::Type::IfKeyword);
		case 'l': return checkKeyword(1, 2, "et", Token::Type::LetKeyword);
		case 'm': return checkKeyword(1, 5, "odule", Token::Type::ModuleKeyword);
		case 'n': return checkKeyword(1, 2, "il", Token::Type::NilKeyword);
		case 'r': return checkKeyword(1, 5, "eturn", Token::Type::ReturnKeyword);
		case 't': return checkKeyword(1, 3, "rue", Token::Type::TrueKeyword);
		case 'w': return checkKeyword(1, 4, "hile", Token::Type::WhileKeyword);
	}

	return Token::Type::Identifier;
}

Token Lexer::identifierOrKeyword() {
	while (isalnum(peek()) || peek() == '_') {
		advance();
	}

	return Token{
		identifierOrKeywordType(),
		m_source.substr(m_start, m_current - m_start),
		m_line,
		m_col - (m_current - m_start - 1)
	};
}

Token Lexer::characterLiteral() {
	advance();
	if (!match('\'')) {
		return error("Expected \"'\" after character literal");
	}

	return Token{
		Token::Type::CharLiteral,
		m_source.substr(m_start + 1, m_current - m_start - 2),
		m_line,
		m_col - 2
	};
}

Token Lexer::stringLiteral() {
	char c;

	// TODO: implement multi-line continuation

	while (true) {
		c = peek();

		if (std::isalnum(c) || std::isspace(c)) {
			advance();
		} else if (c == '\\') {
			// Handle escape sequence
			advance();
			advance();
		} else {
			break;
		}
	}

	if (!match('"')) {
		return error("Expected '\"' after string literal");
	}

	return Token{
		Token::Type::StringLiteral,
		m_source.substr(m_start + 1, m_current - m_start - 2),
		m_line,
		m_col - (m_current - m_start - 1)
	};
}

Token Lexer::next() {
	if (m_dedent_emit_count > 0) {
		m_dedent_emit_count--;
		return simple(Token::Type::Dedent);
	}

	if (m_line_begin) {
		Token indent_or_dedent;
		m_line_begin = false;
		if (indentDedent(indent_or_dedent)) {
			return indent_or_dedent;
		}
	}

	char c = getFirstNonWhitespace();

	switch(c) {
		case '(': return simple(Token::Type::OpenParen);
		case ')': return simple(Token::Type::CloseParen);
		case '[': return simple(Token::Type::OpenBracket);
		case ']': return simple(Token::Type::CloseBracket);
		case '+': return simple(Token::Type::Plus);
		case '-': return simple(Token::Type::Minus);
		case '*': return simple(match('*') ? Token::Type::StarStar : Token::Type::Star);
		case '/': return simple(Token::Type::Slash);
		case '>': return simple(
			match('>') ? Token::Type::GreaterGreater : (
				match('=') ? Token::Type::GreaterEqual : Token::Type::Greater
			)
		);
		case '<': return simple(
			match('<') ? Token::Type::LessLess : (
				match('=') ? Token::Type::LessEqual : Token::Type::Less
			)
		);
		case '=': return simple(match('=') ? Token::Type::EqualEqual : Token::Type::Equal);
		case '!': return simple(match('=') ? Token::Type::NotEqual : Token::Type::Not);
		case '&': return simple(match('&') ? Token::Type::AndAnd : Token::Type::And);
		case '|': return simple(match('|') ? Token::Type::PipePipe : Token::Type::Pipe);
		case '~': return simple(Token::Type::Tilde);
		case '^': return simple(Token::Type::Carat);
		case ':': return simple(Token::Type::Colon);
		case '?': return simple(Token::Type::Question);
		case ',': return simple(Token::Type::Comma);
		
		case '\0': return simple(Token::Type::Eof);
		case '\n': return endOfLine();
		case '\'': return characterLiteral();
		case '"': return stringLiteral();

		default: {
		if (std::isdigit(c)) {
			return number();
		} else if (std::isalpha(c) || c == '_') {
			return identifierOrKeyword();
		} else {
			return error("Unknown character");
		}
			 
		}
	}
}
	
} // namespace Nitro
