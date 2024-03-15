#pragma once

#include "../global/defs.hpp"
#include "../Lexer/Lexer.hpp"
#include "ASTVisitor.hpp"

namespace Nitro {

class ASTNode {
public:
	explicit ASTNode(Token tok);

	virtual ~ASTNode() = 0;

	friend class ASTVisitor;

	virtual void visit(ASTVisitor& visitor) = 0;

	Token m_tok;   // Lexer token used for debugging purposes
		       // Gettine line, col, etc.
};

} // namespace Nitro

