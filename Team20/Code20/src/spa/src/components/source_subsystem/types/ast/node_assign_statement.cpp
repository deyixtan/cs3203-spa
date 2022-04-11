#include "node_assign_statement.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_variable.h"

namespace source {

AssignStatementNode::AssignStatementNode(String &stmt_no, VariableNodePtr lhs, ExpressionNodePtr rhs)
    : StatementNode(stmt_no), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

VariableNodePtr AssignStatementNode::GetLhs() {
  return m_lhs;
}

ExpressionNodePtr AssignStatementNode::GetRhs() {
  return m_rhs;
}

void AssignStatementNode::Accept(DesignExtractorPtr design_extractor) {
  AssignStatementNodePtr derived_ptr = std::dynamic_pointer_cast<AssignStatementNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void AssignStatementNode::Accept(CfgBuilderPtr cfg_builder) {
  AssignStatementNodePtr derived_ptr = std::dynamic_pointer_cast<AssignStatementNode>(shared_from_this());
  cfg_builder->Visit(derived_ptr);
}

bool AssignStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const AssignStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no &&
      *m_lhs == *(casted_other->m_lhs) &&
      *m_rhs == *(casted_other->m_rhs);
}

}
