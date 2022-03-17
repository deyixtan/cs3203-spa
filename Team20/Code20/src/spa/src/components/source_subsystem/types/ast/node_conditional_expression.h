#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_

#include "node.h"
#include "components/source_subsystem/populator.h"

class DesignExtractor;

enum class ConditionalType {
  BOOLEAN,
  NOT,
  RELATIONAL
};

class ConditionalExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ConditionalType GetConditionalType() = 0;
  [[nodiscard]] virtual bool operator==(const ConditionalExpressionNode &other) const = 0;
  virtual std::string Accept(DesignExtractor *de, bool is_uses) = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_
