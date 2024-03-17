#pragma once

#include <iostream>
#include <string>

#include "ASTVisitor.hpp"

namespace Nitro {

// Prints the AST to the terminal in a pleasing manner
class ASTPrettyPrinter : public ASTVisitor {
public:
	ASTPrettyPrinter(std::ostream& os, int tabs = 0);

	void visit(ASTNodeConstant<std::int64_t>& node) override;

	void visit(ASTNodeConstant<double>& node) override;

	void visit(ASTNodeConstant<bool>& node) override;

	void visit(ASTNodeConstant<std::string_view>& node) override;

	void visit(ASTNodeConstant<char>& node) override;

	void visit(ASTNodeNil& node) override;

	void visit(ASTNodeBinary& node) override;

	void visit(ASTNodeUnary& node) override;

	void visit(ASTNodeVariableInvokation& node) override;

	void visit(ASTNodeVariableDeclaration& node) override;

	void visit(ASTNodeStatementSet& node) override;

private:
	std::ostream& m_os;
	int m_tabs;
	std::string m_tabstr;
};

} // namespace Nitro

