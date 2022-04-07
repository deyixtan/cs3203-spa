#include "node_read_statement.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_variable.h"

namespace source {

ReadStatementNode::ReadStatementNode(String &stmt_no, VariableNodePtr variable)
    : StatementNode(stmt_no), m_variable(std::move(variable)) {}

VariableNodePtr ReadStatementNode::GetVariable() {
  return m_variable;
}

void ReadStatementNode::Accept(DesignExtractorPtr design_extractor) {
  ReadStatementNodePtr derived_ptr = std::dynamic_pointer_cast<ReadStatementNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

CfgNodePtr ReadStatementNode::Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) {
  ReadStatementNodePtr derived_ptr = std::dynamic_pointer_cast<ReadStatementNode>(shared_from_this());
  return cfg_builder->Visit(derived_ptr, cfg_node);
}

bool ReadStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const ReadStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_variable == *(casted_other->m_variable);
}

}
