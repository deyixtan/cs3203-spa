#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_

#include "node.h"
#include "../populator.h"

enum class ConditionalType {
  BOOLEAN,
  NOT,
  RELATIONAL
};

class ConditionalExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ConditionalType GetConditionalType() = 0;
  [[nodiscard]] virtual std::string Process(Populator populator, std::vector<std::string>* visited, std::shared_ptr<source::CfgGroupNode> cfg_node) = 0;
  [[nodiscard]] virtual bool operator==(const ConditionalExpressionNode &other) const = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONDITIONAL_EXPRESSION_H_
