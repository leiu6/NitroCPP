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
	inline Token advance() {
		m_previous = m_current;
		m_current = m_lexer.next();
		return m_current;
	}

	inline bool match(Token::Type type) {
		if (type == m_current.type) {
			advance();
			return true;
		}
		return false;
	}

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

	Lexer& m_lexer;
	Token m_previous;
	Token m_current;
	std::unique_ptr<ASTNode> m_ast;
	bool m_had_error;
	bool m_panic_mode;
};

}// namespace Nitro

