#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "node_statement.h"

namespace source {

class WhileStatementNode : public StatementNode {
 private:
  ConditionalExpressionNodePtr m_condition;
  StatementListNodePtr m_stmt_list;

 public:
  WhileStatementNode(String stmt_no, ConditionalExpressionNodePtr condition, StatementListNodePtr stmt_list);
  [[nodiscard]] ConditionalExpressionNodePtr GetCondition();
  [[nodiscard]] StatementListNodePtr GetStatementList();
  [[nodiscard]] StmtType GetStatementType() override;
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementNode &other) const;
};

}

#endif //WHILE_STATEMENT_H
