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

Token Lexer::identifierOrKeyword() {
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
