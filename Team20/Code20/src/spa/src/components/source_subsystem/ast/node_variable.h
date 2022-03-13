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
  void Process(Populator populator, std::vector<std::string>* visited) override;
  //void Process(std::vector<std::string> *visited, std::string stmt, std::shared_ptr<ExpressionNode> expr) override;
  //std::string Process(std::vector<std::string> *visited, std::string stmt_num, std::shared_ptr<ExpressionNode> expr, int direction, std::string pattern) override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_
