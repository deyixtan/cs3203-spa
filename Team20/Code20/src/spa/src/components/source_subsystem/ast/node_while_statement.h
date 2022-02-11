#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_WHILE_STATEMENT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_WHILE_STATEMENT_H_

#include "node_statement.h"
#include "node_statement_list.h"
#include "node_conditional_expression.h"

class WhileStatementNode : public StatementNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_condition;
  std::shared_ptr<StatementListNode> m_stmt_list;

 public:
  WhileStatementNode(int line,
                     std::shared_ptr<ConditionalExpressionNode> cond,
                     std::shared_ptr<StatementListNode> stmt_list);
  std::vector<std::shared_ptr<StatementNode>> getStatementList();
  std::shared_ptr<ConditionalExpressionNode> getConditional();
  std::shared_ptr<StatementListNode> getBody();
  StmtType getStatementType();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_WHILE_STATEMENT_H_
