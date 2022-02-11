#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_IF_STATEMENT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_IF_STATEMENT_H_

#include "node_statement.h"
#include "node_statement_list.h"
#include "node_conditional_expression.h"

class IfStatementNode : public StatementNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_condition;
  std::shared_ptr<StatementListNode> m_if_stmt_list;
  std::shared_ptr<StatementListNode> m_else_stmt_list;

 public:
  IfStatementNode(int line,
                  std::shared_ptr<ConditionalExpressionNode> condition,
                  std::shared_ptr<StatementListNode> if_stmt_list,
                  std::shared_ptr<StatementListNode> else_stmt_list);
  std::vector<std::shared_ptr<StatementNode>> getStatementList();
  std::shared_ptr<ConditionalExpressionNode> getConditional();
  std::shared_ptr<StatementListNode> getConsequent();
  std::shared_ptr<StatementListNode> getAlternative();
  StmtType getStatementType();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_IF_STATEMENT_H_
