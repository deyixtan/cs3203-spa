#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"

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
  [[nodiscard]] std::string ToString(int level) override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
