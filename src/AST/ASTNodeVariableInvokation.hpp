#pragma once

#include "ASTNode.hpp"

#include <string_view>
#include <vector>
#include <memory>

namespace Nitro {

class ASTNodeVariableInvokation : public ASTNode {
public:
	ASTNodeVariableInvokation(Token tok, std::vector<std::unique_ptr<ASTNode>> args) : ASTNode(tok), m_identifier(tok.lexeme), m_args(std::move(args)) {}

	~ASTNodeVariableInvokation() override = default;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::string_view m_identifier;
	std::vector<std::unique_ptr<ASTNode>> m_args;
};

} // namespace Nitro
