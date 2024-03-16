#pragma once

#include <memory>

#include "ASTNode.hpp"

namespace Nitro {

class ASTNodeBinary : public ASTNode {
public:
	enum class Type {
		Add,
		Sub,
		Mult,
		Div,
		Pow,

		Greater,
		GreaterEqual,
		RShift,
		Less,
		LessEqual,
		LShift,

		Equality,
		NonEquality,

		BitwiseAnd,
		BitwiseOr,
		BitwiseXor,

		And,
		Or
	};

	ASTNodeBinary(
		Token tok,
		Type type,
		std::unique_ptr<ASTNode> left,
		std::unique_ptr<ASTNode> right
	) : ASTNode(tok), m_type(type), m_left(std::move(left)), m_right(std::move(right)) {}

	~ASTNodeBinary() override = default;

	friend class ASTVisitor;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::unique_ptr<ASTNode> m_left;
	std::unique_ptr<ASTNode> m_right;
	Type m_type;
};

} // namespace Nitro

