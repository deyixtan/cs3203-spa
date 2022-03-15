#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_

#include "node_expression.h"

class VariableNode : public ExpressionNode {
 private:
  std::string m_identifier;
  std::string m_stmt;

 public:
  VariableNode();
  explicit VariableNode(std::string identifier, std::string stmt);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string>* visited, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_
