#ifndef NODE_IF_STATEMENT_H
#define NODE_IF_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class IfStatementNode : public StatementNode {
 private:
  ConditionalExpressionNodePtr m_condition;
  StatementListNodePtr m_if_stmt_list;
  StatementListNodePtr m_else_stmt_list;

 public:
  IfStatementNode(String stmt_no,
                  ConditionalExpressionNodePtr condition,
                  StatementListNodePtr if_stmt_list,
                  StatementListNodePtr else_stmt_list);
  [[nodiscard]] ConditionalExpressionNodePtr GetCondition();
  [[nodiscard]] StatementListNodePtr GetIfStatementList();
  [[nodiscard]] StatementListNodePtr GetElseStatementList();
  [[nodiscard]] StatementNodeStream GetAllStatementList();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
};

}

#endif //NODE_IF_STATEMENT_H
