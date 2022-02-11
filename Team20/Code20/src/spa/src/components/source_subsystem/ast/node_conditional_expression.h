#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_

#include "node.h"

enum class ConditionalType {
  BOOLEAN,
  NOT,
  RELATIONAL
};

class ConditionalExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ConditionalType GetConditionalType();
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_
