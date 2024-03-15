#include "ASTNode.hpp"

namespace Nitro {

class ASTNodeNil : public ASTNode {
public:
	ASTNodeNil(Token tok) : ASTNode(tok) {}

	~ASTNodeNil() override = default;

	friend class ASTVisitor;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	};
};

} // namespace Nitro

