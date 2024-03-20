#pragma once

#include "ASTNode.hpp"

#include <memory>

namespace Nitro {

class ASTNodeFunctionReturn : public ASTNode {
public:
	ASTNodeFunctionReturn(Token tok, std::unique_ptr<ASTNode> expr) :
		ASTNode(tok), m_expr(std::move(expr)) {}

	~ASTNodeFunctionReturn() override = default;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::unique_ptr<ASTNode> m_expr;
};

} // namespace Nitro
