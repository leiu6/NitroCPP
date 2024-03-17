#pragma once

#include "ASTNode.hpp"

#include <memory>
#include <string_view>

namespace Nitro {

class ASTNodeVariableDeclaration : public ASTNode {
public:
	ASTNodeVariableDeclaration(Token tok, std::string_view identifier, std::unique_ptr<ASTNode> assign) : ASTNode(tok), m_identifier(identifier), m_assign(std::move(assign)) {}

	~ASTNodeVariableDeclaration() override = default;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::string_view m_identifier;
	std::unique_ptr<ASTNode> m_assign;
};

} // namespace Nitro
