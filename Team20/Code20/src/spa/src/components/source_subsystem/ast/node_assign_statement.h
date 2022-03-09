#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"
#include "node_constant.h"
#include "node_combination_expression.h"

class AssignStatementNode : public StatementNode {
 private:
  std::shared_ptr<VariableNode> m_identifier;
  std::shared_ptr<ExpressionNode> m_expression;

 public:
  AssignStatementNode(int stmt_no,
                      std::shared_ptr<VariableNode> identifier,
                      std::shared_ptr<ExpressionNode> expression);
  [[nodiscard]] std::shared_ptr<VariableNode> GetIdentifier();
  [[nodiscard]] std::shared_ptr<ExpressionNode> GetExpression();
  [[nodiscard]] StmtType GetStatementType() override;
  void Process(Populator populator, std::vector<std::string> *visited) override;
  void Process(Populator populator, std::vector<std::string> *visited, std::string stmt) override;
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, std::string stmt_num, int direction, std::string pattern) override;
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
