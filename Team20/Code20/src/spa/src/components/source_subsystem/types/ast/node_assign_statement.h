#ifndef NODE_ASSIGN_STATEMENT_H
#define NODE_ASSIGN_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "node_statement.h"

namespace source {

class AssignStatementNode : public StatementNode {
 private:
  VariableNodePtr m_lhs;
  ExpressionNodePtr m_rhs;

 public:
  AssignStatementNode(String stmt_no, VariableNodePtr lhs, ExpressionNodePtr rhs);
  [[nodiscard]] VariableNodePtr GetLhs();
  [[nodiscard]] ExpressionNodePtr GetRhs();
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

}

#endif //NODE_ASSIGN_STATEMENT_H
