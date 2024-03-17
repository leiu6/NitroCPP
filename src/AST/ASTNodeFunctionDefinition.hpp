#pragma once

#include "ASTNode.hpp"

#include <vector>
#include <string_view>
#include <memory>

namespace Nitro {

class ASTNodeFunctionDefinition : public ASTNode {
public:
	ASTNodeFunctionDefinition(Token tok, std::string_view id, std::vector<std::string_view> args, std::unique_ptr<ASTNode> contents) :
		ASTNode(tok), m_identifier(id), m_args(std::move(args)), m_contents(std::move(contents)) {}

	~ASTNodeFunctionDefinition() override = default;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	std::string_view m_identifier;
	std::vector<std::string_view> m_args;
	std::unique_ptr<ASTNode> m_contents;
};

} // namespace Nitro
