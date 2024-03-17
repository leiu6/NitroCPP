#pragma once

#include <memory>
#include <iostream>

#include "../Lexer/Lexer.hpp"
#include "../AST/ASTNode.hpp"

namespace Nitro {

class Parser {
public:
	explicit Parser(Lexer& lexer);

	std::unique_ptr<ASTNode> parse();

private:
	inline void errorCurrent(std::string_view msg) {
		if (m_had_error) {
			return;
		}

		m_had_error = true;
		m_panic_mode = true;

		std::cerr << "Error: " << m_current.line << ":" << m_current.col << ": " << msg << "\n";

	}

	inline Token advance() {
		m_previous = m_current;
		m_current = m_next;
		m_next = m_lexer.next();
		return m_current;
	}

	inline bool match(Token::Type type) {
		if (type == m_current.type) {
			advance();
			return true;
		}
		return false;
	}

	inline bool peek(Token::Type type) {
		if (type == m_current.type) {
			return true;
		}
		return false;
	}

	// I am a bit ashamed of this hack
	inline bool peekRange(Token::Type lower, Token::Type upper) {
		unsigned ilower, iupper, icurrent;

		ilower = static_cast<unsigned>(lower);
		iupper = static_cast<unsigned>(upper);
		icurrent = static_cast<unsigned>(m_current.type);

		if ((icurrent >= ilower) && (icurrent <= iupper)) {
			return true;
		}
		
		return false;
	}

	inline bool match_next(Token::Type type) {
		if (type == m_next.type) {
			advance();
			return true;
		}
		return false;
	}

	std::unique_ptr<ASTNode> parseStatements();

	std::unique_ptr<ASTNode> parseStatement();

	std::unique_ptr<ASTNode> parseVariableDeclaration();
	std::unique_ptr<ASTNode> parseExpressionStatement();


	std::unique_ptr<ASTNode> parseExpression();

	std::unique_ptr<ASTNode> parseOrAnd();

	std::unique_ptr<ASTNode> parseBitwiseOrAndXor();

	std::unique_ptr<ASTNode> parseEqualNotEqual();

	std::unique_ptr<ASTNode> parseGreaterLessAndEqual();

	std::unique_ptr<ASTNode> parseLShiftRShift();

	std::unique_ptr<ASTNode> parsePlusMinus();

	std::unique_ptr<ASTNode> parseMultiplyDivide();

	std::unique_ptr<ASTNode> parsePow();

	std::unique_ptr<ASTNode> parsePrefix();

	std::unique_ptr<ASTNode> parsePrimary();

	std::unique_ptr<ASTNode> parseVariableCall();

	Lexer& m_lexer;
	Token m_previous;
	Token m_current;
	Token m_next;
	std::unique_ptr<ASTNode> m_ast;
	bool m_had_error;
	bool m_panic_mode;
};

}// namespace Nitro

