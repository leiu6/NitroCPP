#include "ASTPrettyPrinter.hpp"

#include <string>

#include "ASTNode.hpp"
#include "ASTNodeConstant.hpp"
#include "ASTNodeNil.hpp"
#include "ASTNodeBinary.hpp"
#include "ASTNodeUnary.hpp"
#include "ASTNodeVariableInvokation.hpp"
#include "ASTNodeVariableDeclaration.hpp"
#include "ASTNodeStatementSet.hpp"
#include "ASTNodeConditional.hpp"

namespace Nitro {

ASTPrettyPrinter::ASTPrettyPrinter(std::ostream& os, int tabs) :
	m_os(os), m_tabs(tabs), m_tabstr(tabs, '\t') {}

void ASTPrettyPrinter::visit(ASTNodeConstant<std::int64_t>& node) {
	m_os << m_tabstr << "Constant: {\n";
	m_os << m_tabstr << "Type: Integer\n";
	m_os << m_tabstr << "Value: " << node.m_value << "\n";
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeConstant<double>& node) {
	m_os << m_tabstr << "Constant: {\n";
	m_os << m_tabstr << "Type: Float\n";
	m_os << m_tabstr << "Value: " << node.m_value << "\n";
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeConstant<bool>& node) {
	m_os << m_tabstr << "Constant: {\n";
	m_os << m_tabstr << "Type: Boolean\n";
	m_os << m_tabstr << "Value: ";

	if (node.m_value) {
		m_os << "True\n";
	} else {
		m_os << "False\n";
	}

	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeConstant<std::string_view>& node) {
	m_os << m_tabstr << "Constant: {\n";
	m_os << m_tabstr << "Type: String\n";
	m_os << m_tabstr << "Value: \"" << node.m_value << "\"\n";
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeConstant<char>& node) {
	m_os << m_tabstr << "Constant: {\n";
	m_os << m_tabstr << "Type: Char\n";
	m_os << m_tabstr << "Value: '" << node.m_value << "'\n";
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeNil& node) {
	Token tk = node.m_tok;
	(void)tk;
	m_os << m_tabstr << "Constant: {\n";
	m_os << m_tabstr << "Type: Nil\n";
	m_os << m_tabstr << "Value: Nil\n";
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeBinary& node) {
	m_os << m_tabstr << "Binary Op: {\n";
	m_os << m_tabstr << "Type: " << static_cast<int>(node.m_type) << "\n";
	m_os << m_tabstr << "Lhs -> {\n";
	ASTPrettyPrinter printer(m_os, m_tabs + 1);
	if (node.m_left) {
		node.m_left->visit(printer);
	} else {
		m_os << m_tabstr << "nullnode\n";
	}
	m_os << m_tabstr << "}\n";
	m_os << m_tabstr << "Rhs -> {\n";
	if (node.m_right) {
		node.m_right->visit(printer);
	} else {
		m_os << m_tabstr << "nullnode\n";
	}
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeUnary& node) {
	m_os << m_tabstr << "Unary Op: {\n";
	m_os << m_tabstr << "Type: " << static_cast<int>(node.m_type) << "\n";
	m_os << m_tabstr << "Branch -> {\n";
	ASTPrettyPrinter printer(m_os, m_tabs + 1);
	if (node.m_branch) {
		node.m_branch->visit(printer);
	} else {
		m_os << m_tabstr << "nullnode\n";
	}
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeVariableInvokation& node) {
	m_os << m_tabstr << "Invoke var: {\n";
	m_os << m_tabstr << "Name: " << node.m_identifier << "\n";
	m_os << m_tabstr << "N Args: " << node.m_args.size() << "\n";

	for (auto & arg : node.m_args) {
		ASTPrettyPrinter printer(m_os, m_tabs + 1);
		arg->visit(printer);
	}

	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeVariableDeclaration& node) {
	m_os << m_tabstr << "Declare var: {\n";
	m_os << m_tabstr << "Name: " << node.m_identifier << "\n";
	m_os << m_tabstr << "Assign -> {\n";
	ASTPrettyPrinter printer(m_os, m_tabs + 1);
	node.m_assign->visit(printer);
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeStatementSet& node) {
	m_os << m_tabstr << "Statements: {\n";
	ASTPrettyPrinter printer(m_os, m_tabs + 1);

	for (auto& statement : node.m_statements) {
		if (statement) {
			statement->visit(printer);
		}
		
		m_os << m_tabstr << "\n";
	}

	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeConditional& node) {
	ASTPrettyPrinter printer(m_os, m_tabs + 1);

	m_os << m_tabstr << "Conditional: {\n";

	for (auto& condition : node.m_conditions) {
		m_os << m_tabstr << "if -> {\n";
		condition.first->visit(printer);
		m_os << m_tabstr << "} then -> {\n";
		condition.second->visit(printer);
		m_os << m_tabstr << "}\n";
	}

	if (node.m_else.first) {
		m_os << m_tabstr << "else -> {\n";
		node.m_else.first->visit(printer);
		m_os << m_tabstr << "} then -> {\n";
		node.m_else.second->visit(printer);
		m_os << m_tabstr << "}\n";
	}
}

} // namespace Nitro
