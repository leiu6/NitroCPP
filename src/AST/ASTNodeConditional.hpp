#pragma once

#include "ASTNode.hpp"

#include <vector>
#include <memory>
#include <utility>

namespace Nitro {

class ASTNodeConditional : public ASTNode {
public:
	using Conditional = std::pair<std::unique_ptr<ASTNode>, std::unique_ptr<ASTNode>>;

	ASTNodeConditional(Token tok, std::vector<Conditional> conditions, std::unique_ptr<ASTNode> Else) :
		ASTNode(tok), m_conditions(std::move(conditions)), m_else_statement(std::move(Else)) {}

	~ASTNodeConditional() override = default;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::vector<Conditional> m_conditions;
	std::unique_ptr<ASTNode> m_else_statement;
};

} // namespace Nitro
