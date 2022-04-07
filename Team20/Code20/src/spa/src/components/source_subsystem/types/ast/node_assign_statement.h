#ifndef NODE_ASSIGN_STATEMENT_H
#define NODE_ASSIGN_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class AssignStatementNode : public StatementNode {
 private:
  VariableNodePtr m_lhs;
  ExpressionNodePtr m_rhs;

 public:
  AssignStatementNode(String &stmt_no, VariableNodePtr lhs, ExpressionNodePtr rhs);
  [[nodiscard]] VariableNodePtr GetLhs();
  [[nodiscard]] ExpressionNodePtr GetRhs();
  void Accept(DesignExtractorPtr design_extractor) override;
  void Accept(CfgBuilderPtr cfg_builder) override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

}

#endif //NODE_ASSIGN_STATEMENT_H
