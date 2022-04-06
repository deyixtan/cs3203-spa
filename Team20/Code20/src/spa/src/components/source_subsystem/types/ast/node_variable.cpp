#include "node_variable.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

VariableNode::VariableNode(String name, String stmt_no) : m_name(std::move(name)), m_stmt_no(std::move(stmt_no)) {}

String VariableNode::GetName() {
  return m_name;
}

String VariableNode::GetPatternFormat() {
  return "(" + m_name + ")";
}

void VariableNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<VariableNode>(shared_from_this()));
}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_name == casted_other->m_name;
}

}
