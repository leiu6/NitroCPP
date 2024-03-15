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

char Lexer::peek() {
	return m_source[m_current];		
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
	return token;
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

Token Lexer::identifierOrKeyword() {
	char c;

	while (isalnum(peek()) || peek() == '_') {
		advance();
	}

	return Token{
		Token::Type::Identifier,
		m_source.substr(m_start, m_current - m_start),
		m_line,
		m_col - (m_current - m_start - 1)
	};
}

Token Lexer::next() {
	char c;

	// Skip whitespace
	do {
		m_start = m_current;
		c = advance();
	} while (c == ' ');

	switch(c) {
		case '(': return simple(Token::Type::OpenParen);
		case ')': return simple(Token::Type::CloseParen);
		case '+': return simple(Token::Type::Plus);
		case '-': return simple(Token::Type::Minus);
		case '*': return simple(match('*') ? Token::Type::StarStar : Token::Type::Star);
		case '/': return simple(Token::Type::Slash);
		
		case '\0': return simple(Token::Type::Eof);
		case '\n': return endOfLine();

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
