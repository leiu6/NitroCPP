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
	return parseOrAnd();
}

std::unique_ptr<ASTNode> Parser::parseOrAnd() {
	auto lhs = parseBitwiseOrAndXor();

	for (;;) {
		if (match(Token::Type::PipePipe)) {
			auto rhs = parseBitwiseOrAndXor();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Or,
				std::move(lhs),
				std::move(rhs)
				);
		}
		else if (match(Token::Type::AndAnd)) {
			auto rhs = parseBitwiseOrAndXor();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::And,
				std::move(lhs),
				std::move(rhs)
				);
		} else {
			break;
		}
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parseBitwiseOrAndXor() {
	auto lhs = parseEqualNotEqual();

	for (;;) {
		if (match(Token::Type::Pipe)) {
			auto rhs = parseEqualNotEqual();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::BitwiseOr,
				std::move(lhs),
				std::move(rhs)
				);
		}
		else if (match(Token::Type::And)) {
			auto rhs = parseEqualNotEqual();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::BitwiseAnd,
				std::move(lhs),
				std::move(rhs)
				);
		} else if (match(Token::Type::Carat)) {
			auto rhs = parseEqualNotEqual();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::BitwiseXor,
				std::move(lhs),
				std::move(rhs)
			);
		} else {
			break;
		}
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parseEqualNotEqual() {
	auto lhs = parseGreaterLessAndEqual();

	for (;;) {
		if (match(Token::Type::EqualEqual)) {
			auto rhs = parseGreaterLessAndEqual();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Equality,
				std::move(lhs),
				std::move(rhs)
				);
		}
		else if (match(Token::Type::NotEqual)) {
			auto rhs = parseGreaterLessAndEqual();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::NonEquality,
				std::move(lhs),
				std::move(rhs)
				);
		} else {
			break;
		}
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parseGreaterLessAndEqual() {
	auto lhs = parseLShiftRShift();

	for (;;) {
		if (match(Token::Type::Greater)) {
			auto rhs = parseLShiftRShift();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Greater,
				std::move(lhs),
				std::move(rhs)
				);
		} else if (match(Token::Type::GreaterEqual)) {
			auto rhs = parseLShiftRShift();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::GreaterEqual,
				std::move(lhs),
				std::move(rhs)
				);
		} else if (match(Token::Type::Less)) {
			auto rhs = parseLShiftRShift();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::Less,
				std::move(lhs),
				std::move(rhs)
				);
		} else if (match(Token::Type::LessEqual)) {
			auto rhs = parseLShiftRShift();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::LessEqual,
				std::move(lhs),
				std::move(rhs)
				);
		} else {
			break;
		}
	}

	return lhs;
}

std::unique_ptr<ASTNode> Parser::parseLShiftRShift() {
	auto lhs = parsePlusMinus();

	for (;;) {
		if (match(Token::Type::GreaterGreater)) {
			auto rhs = parsePlusMinus();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::RShift,
				std::move(lhs),
				std::move(rhs)
				);
		}
		else if (match(Token::Type::LessLess)) {
			auto rhs = parsePlusMinus();
			lhs = std::make_unique<ASTNodeBinary>(
				m_previous,
				ASTNodeBinary::Type::LShift,
				std::move(lhs),
				std::move(rhs)
				);
		}
		else {
			break;
		}
	}

	return lhs;
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
	} else if (match(Token::Type::Not)) {
		return std::make_unique<ASTNodeUnary>(
			m_previous,
			ASTNodeUnary::Type::Not,
			parsePrefix()
		);
	} else if (match(Token::Type::Tilde)) {
		return std::make_unique<ASTNodeUnary>(
			m_previous,
			ASTNodeUnary::Type::BitwiseNot,
			parsePrefix()
		);
	}

	return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
	if (match(Token::Type::FloatLiteral)) {
		double value = std::strtod(m_previous.lexeme.data(), nullptr);
		return std::make_unique<ASTNodeFloat64>(m_previous, value);		
	} else if (match(Token::Type::IntegerLiteral)) {
		std::int64_t value = std::strtoll(m_previous.lexeme.data(), nullptr, 10);
		return std::make_unique<ASTNodeInt64>(m_previous, value);
	} else if (match(Token::Type::OpenParen)) {
		auto expr = parseExpression();
		if (!match(Token::Type::CloseParen)) {
			std::cerr << "Expected ')' at end of expression" << std::endl;
			return nullptr;
		}
		return expr;
	} else {
		std::cerr << "Unexpected!!" << std::endl;
		return nullptr;	
	}
}

} // namespace Nitro

