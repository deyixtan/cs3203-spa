#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_WHILE_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_WHILE_STATEMENT_H_

#include "node_statement.h"
#include "node_conditional_expression.h"
#include "node_statement_list.h"

class WhileStatementNode : public StatementNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_condition;
  std::shared_ptr<StatementListNode> m_stmt_list;

 public:
  WhileStatementNode(int stmt_no,
                     std::shared_ptr<ConditionalExpressionNode> condition,
                     std::shared_ptr<StatementListNode> stmt_list);
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetCondition();
  [[nodiscard]] std::shared_ptr<StatementListNode> GetStatementList();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] void Process(Populator populator, std::vector<std::string>* visited) override;
  [[nodiscard]] bool operator==(const StatementNode &other) const;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_WHILE_STATEMENT_H_
