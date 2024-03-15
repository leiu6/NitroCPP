#include "ASTPrettyPrinter.hpp"

#include <string>

#include "ASTNode.hpp"
#include "ASTNodeConstant.hpp"
#include "ASTNodeNil.hpp"
#include "ASTNodeBinary.hpp"
#include "ASTNodeUnary.hpp"

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
	m_os << m_tabstr << "Value: " << node.m_value << "\n";
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
	node.m_left->visit(printer);
	m_os << m_tabstr << "}\n";
	m_os << m_tabstr << "Rhs -> {\n";
	node.m_right->visit(printer);
	m_os << m_tabstr << "}\n";
}

void ASTPrettyPrinter::visit(ASTNodeUnary& node) {
	m_os << m_tabstr << "Unary Op: {\n";
	m_os << m_tabstr << "Type: " << static_cast<int>(node.m_type) << "\n";
	m_os << m_tabstr << "Branch -> {\n";
	ASTPrettyPrinter printer(m_os, m_tabs + 1);
	node.m_branch->visit(printer);
	m_os << m_tabstr << "}\n";
}

} // namespace Nitro
