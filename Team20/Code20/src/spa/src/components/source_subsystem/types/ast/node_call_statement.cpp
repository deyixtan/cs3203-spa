#include "node_call_statement.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/iterator/cfg_builder.h"

namespace source {

CallStatementNode::CallStatementNode(String &stmt_no, String caller_name, String callee_name)
    : StatementNode(stmt_no), m_caller_name(std::move(caller_name)), m_callee_name(std::move(callee_name)) {}

String CallStatementNode::GetCallerName() {
  return m_caller_name;
}

String CallStatementNode::GetCalleeName() {
  return m_callee_name;
}

void CallStatementNode::Accept(DesignExtractorPtr design_extractor) {
  CallStatementNodePtr derived_ptr = std::dynamic_pointer_cast<CallStatementNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void CallStatementNode::Accept(CfgBuilderPtr cfg_builder) {
  CallStatementNodePtr derived_ptr = std::dynamic_pointer_cast<CallStatementNode>(shared_from_this());
  cfg_builder->Visit(derived_ptr);
}

bool CallStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const CallStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && m_caller_name == casted_other->m_caller_name && m_callee_name == casted_other->m_callee_name;
}

}
