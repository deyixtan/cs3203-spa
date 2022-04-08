#include "node_print_statement.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_variable.h"

namespace source {

PrintStatementNode::PrintStatementNode(String &stmt_no, VariableNodePtr variable)
    : StatementNode(stmt_no), m_variable(std::move(variable)) {}

VariableNodePtr PrintStatementNode::GetVariable() {
  return m_variable;
}

void PrintStatementNode::Accept(DesignExtractorPtr design_extractor) {
  PrintStatementNodePtr derived_ptr = std::dynamic_pointer_cast<PrintStatementNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void PrintStatementNode::Accept(CfgBuilderPtr cfg_builder) {
  PrintStatementNodePtr derived_ptr = std::dynamic_pointer_cast<PrintStatementNode>(shared_from_this());
  cfg_builder->Visit(derived_ptr);
}

bool PrintStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const PrintStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_variable == *(casted_other->m_variable);
}

}
