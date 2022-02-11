#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"

class AssignStatementNode : public StatementNode
{
 private:
  std::shared_ptr<VariableNode> m_id;
  std::shared_ptr<ExpressionNode> m_expression;

 public:
  AssignStatementNode(int line, std::shared_ptr<VariableNode> id, std::shared_ptr<ExpressionNode> expr);
  std::shared_ptr<VariableNode> getId();
  std::shared_ptr<ExpressionNode> getExpr();
  StmtType getStatementType();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
