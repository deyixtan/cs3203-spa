#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_IF_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_IF_STATEMENT_H_

#include "node_statement.h"
#include "node_conditional_expression.h"
#include "node_statement_list.h"
#include "components/source_subsystem/pkb_client.h"

class IfStatementNode : public StatementNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_condition;
  std::shared_ptr<StatementListNode> m_if_stmt_list;
  std::shared_ptr<StatementListNode> m_else_stmt_list;

 public:
  IfStatementNode(int stmt_no,
                  std::shared_ptr<ConditionalExpressionNode> condition,
                  std::shared_ptr<StatementListNode> if_stmt_list,
                  std::shared_ptr<StatementListNode> else_stmt_list);
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetCondition();
  [[nodiscard]] std::shared_ptr<StatementListNode> GetIfStatementList();
  [[nodiscard]] std::shared_ptr<StatementListNode> GetElseStatementList();
  [[nodiscard]] std::vector<std::shared_ptr<StatementNode>> GetAllStatementList();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de, std::string proc_name);
  std::shared_ptr<CfgNode> Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_IF_STATEMENT_H_
