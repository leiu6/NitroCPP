#include "Lexer.hpp"

#include <cctype>

namespace Nitro {

Lexer::Lexer(std::string_view source)
	: m_source(source), m_current(0), m_start(0), m_line(1), m_col(0) {}

char Lexer::advance() {
	char c = m_source[m_current++];

	if (c == '\n') {
		m_line++;
		m_col = 0;	
	}

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

Token Lexer::next() {
	char c;

	// Skip whitespace
	do {
		m_start = m_current;
		c = advance();
	} while (c == ' ' || c == '\n' || c == '\t');

	switch(c) {
		case '(': return simple(Token::Type::OpenParen);
		case ')': return simple(Token::Type::CloseParen);
		case '+': return simple(Token::Type::Plus);
		case '-': return simple(Token::Type::Minus);
		case '*': return simple(match('*') ? Token::Type::StarStar : Token::Type::Star);
		case '/': return simple(Token::Type::Slash);

		case '\0': return simple(Token::Type::Eof);
		
		default: {
		if (std::isdigit(c)) {
			return number();
		} else {
			return error("Unknown character");
		}
			 
		}
	}
}
	
} // namespace Nitro
