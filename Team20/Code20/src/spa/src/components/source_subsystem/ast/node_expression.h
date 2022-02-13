#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

#include "node.h"

enum class ExpressionType {
  CONSTANT,
  COMBINATION,
  VARIABLE
};

class ExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ExpressionType GetExpressionType() = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
