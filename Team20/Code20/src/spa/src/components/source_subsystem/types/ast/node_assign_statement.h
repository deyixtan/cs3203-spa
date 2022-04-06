#ifndef NODE_ASSIGN_STATEMENT_H
#define NODE_ASSIGN_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class AssignStatementNode : public StatementNode, std::enable_shared_from_this<AssignStatementNode> {
 private:
  VariableNodePtr m_lhs;
  ExpressionNodePtr m_rhs;

 public:
  AssignStatementNode(String &stmt_no, VariableNodePtr lhs, ExpressionNodePtr rhs);
  [[nodiscard]] VariableNodePtr GetLhs();
  [[nodiscard]] ExpressionNodePtr GetRhs();
  void Accept(DesignExtractorPtr design_extractor) override;
  CfgNodePtr Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  [[nodiscard]] StmtType GetStatementType() override;
};

}

#endif //NODE_ASSIGN_STATEMENT_H
