#include "Parser.hpp"

#include <cctype>

#include "../AST/ASTNodeBinary.hpp"
#include "../AST/ASTNodeUnary.hpp"
#include "../AST/ASTNodeConstant.hpp"
#include "../AST/ASTNodeNil.hpp"

namespace Nitro {

Parser::Parser(Lexer& lexer) : m_lexer(lexer) {
	m_previous = m_current = m_lexer.next();
	m_ast      = nullptr;
	m_had_error = false;
	m_panic_mode = false;
}

std::unique_ptr<ASTNode> Parser::parse() {
	return parseExpression();
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
	return parsePlusMinus();	
}

std::unique_ptr<ASTNode> Parser::parsePlusMinus() {
	auto lhs = parseMultiplyDivide();

	for (;;) {
		if (match(Token::Type::Plus)) {
			auto rhs = parseMultiplyDivide();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Add,
				std::move(lhs),
				std::move(rhs)
			);
		} else if (match(Token::Type::Minus)) {
			auto rhs = parseMultiplyDivide();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Sub,
				std::move(lhs),
				std::move(rhs)
			);
		} else {
			break;
		}
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parseMultiplyDivide() {
	auto lhs = parsePow();

	for (;;) {
		if (match(Token::Type::Star)) {
			auto rhs = parsePow();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Mult,
				std::move(lhs),
				std::move(rhs)
			);
		} else if (match(Token::Type::Slash)) {
			auto rhs = parsePow();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Div,
				std::move(lhs),
				std::move(rhs)
			);
		} else {
			break;
		}
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parsePow() {
	auto lhs = parsePrefix();

	for (;;) {
		if (match(Token::Type::StarStar)) {
			auto rhs = parsePrefix();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Pow,
				std::move(lhs),
				std::move(rhs)
			);
		} else {
			break;
		} 
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parsePrefix() {
	if (match(Token::Type::Plus)) {
		// This one doesn't actually do anything
		return std::make_unique<ASTNodeUnary>(
			m_previous,
			ASTNodeUnary::Type::Plus,
			parsePrefix()
		);
	} else if (match(Token::Type::Minus)) {
		return std::make_unique<ASTNodeUnary>(
			m_previous,
			ASTNodeUnary::Type::Negate,
			parsePrefix()		
		);
	}

	return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
	if (match(Token::Type::FloatLiteral)) {
		double value = std::strtod(m_previous.lexeme.cbegin(), nullptr);
		return std::make_unique<ASTNodeFloat64>(m_previous, value);		
	} else if (match(Token::Type::IntegerLiteral)) {
		std::int64_t value = std::strtoll(m_previous.lexeme.cbegin(), nullptr, 10);
		return std::make_unique<ASTNodeInt64>(m_previous, value);
	} else {
		std::cerr << "Unexpected!!" << std::endl;
		return nullptr;	
	}
}

} // namespace Nitro

