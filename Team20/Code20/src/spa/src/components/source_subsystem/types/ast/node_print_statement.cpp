#include "node_print_statement.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"
#include "components/source_subsystem/iterator/cfg_builder.h"

namespace source {

PrintStatementNode::PrintStatementNode(String &stmt_no, VariableNodePtr variable)
    : StatementNode(stmt_no), m_variable(std::move(variable)) {}

VariableNodePtr PrintStatementNode::GetVariable() {
  return m_variable;
}

void PrintStatementNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<PrintStatementNode>(shared_from_this()));
}

CfgNodePtr PrintStatementNode::Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) {
  return cfg_builder->Visit(std::dynamic_pointer_cast<PrintStatementNode>(shared_from_this()), cfg_node);
}

bool PrintStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const PrintStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_variable == *(casted_other->m_variable);
}

}
