#include "ASTNode.hpp"

#include <cstdint>
#include <string_view>
#include <climits>

namespace Nitro {

template <typename T>
class ASTNodeConstant : public ASTNode {
public:
	ASTNodeConstant(Token tok, T value) : ASTNode(tok), m_value(value) {}

	~ASTNodeConstant() override = default;

	friend class ASTVisitor;

	void visit(ASTVisitor& visitor) override {
		visitor.visit(*this);
	}

	T m_value;
};

// Types of constants
using ASTNodeFloat64 = ASTNodeConstant<double>;
static_assert(sizeof(double) * CHAR_BIT == 64, "double is not a 64 bit value");

using ASTNodeInt64 = ASTNodeConstant<std::int64_t>;

using ASTNodeBool = ASTNodeConstant<bool>;

using ASTNodeString = ASTNodeConstant<std::string_view>;

using ASTNodeChar = ASTNodeConstant<char>;

} // namespace Nitro

