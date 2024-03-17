#pragma once

#include "ASTNode.hpp"

#include <vector>
#include <memory>

namespace Nitro {

class ASTNodeStatementSet : public ASTNode {
public:
	ASTNodeStatementSet(Token tok, std::vector<std::unique_ptr<ASTNode>> statements) :
		ASTNode(tok), m_statements(std::move(statements)) {}

	~ASTNodeStatementSet() override = default;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::vector<std::unique_ptr<ASTNode>> m_statements;
};

} // namespace Nitro
