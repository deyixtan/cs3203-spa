#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

#include "node.h"
#include "../populator.h"

enum class ExpressionType {
  CONSTANT,
  COMBINATION,
  VARIABLE
};

class ExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ExpressionType GetExpressionType() = 0;
  [[nodiscard]] virtual std::string Process(Populator populator, std::vector<std::string>* visited, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) = 0;
  [[nodiscard]] virtual bool operator==(const ExpressionNode &other) const = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
