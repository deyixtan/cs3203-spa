#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

#include "node.h"

enum class ExpressionType {
  CONSTANT,
  COMBINATION,
  VARIABLE,
  NONE
};

class ExpressionNode : public Node {
 public:
  virtual ExpressionType getExpressionType();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
