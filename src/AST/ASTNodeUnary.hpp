#pragma once

#include <memory>

#include "ASTNode.hpp"

namespace Nitro {

class ASTNodeUnary : public ASTNode {
public:
	enum class Type {
		Plus, /* Does nothing! */
		Negate,
		Not,
		BitwiseNot
	};

	ASTNodeUnary(Token tok, Type type, std::unique_ptr<ASTNode> branch) : 
		ASTNode(tok), m_type(type), m_branch(std::move(branch)) {}

	~ASTNodeUnary() override = default;

	friend class ASTVisitor;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	Type m_type;
	std::unique_ptr<ASTNode> m_branch;
};

} // namespace Nitro

