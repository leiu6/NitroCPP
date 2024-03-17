#pragma once

#include <cstdint>
#include <string_view>

namespace Nitro {

/* Forward */
template <typename T> class ASTNodeConstant;

class ASTNodeNil;
class ASTNodeBinary;
class ASTNodeUnary;
class ASTNodeVariableInvokation;
class ASTNodeVariableDeclaration;
class ASTNodeStatementSet;

class ASTVisitor {
public:
	virtual void visit(ASTNodeConstant<std::int64_t>& node) = 0;

	virtual void visit(ASTNodeConstant<double>& node) = 0;

	virtual void visit(ASTNodeConstant<bool>& node) = 0;

	virtual void visit(ASTNodeConstant<std::string_view>& node) = 0;

	virtual void visit(ASTNodeConstant<char>& node) = 0;

	virtual void visit(ASTNodeNil& node) = 0;

	virtual void visit(ASTNodeBinary& node) = 0;

	virtual void visit(ASTNodeUnary& node) = 0;

	virtual void visit(ASTNodeVariableInvokation& node) = 0;

	virtual void visit(ASTNodeVariableDeclaration& node) = 0;

	virtual void visit(ASTNodeStatementSet& node) = 0;
};

} // namespace Nitro

