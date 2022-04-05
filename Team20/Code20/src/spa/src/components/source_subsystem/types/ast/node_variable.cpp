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

String VariableNode::Accept(DesignExtractor *design_extractor, String proc_name, bool is_uses) {
  design_extractor->GetPkbClient()->PopulateVars(design_extractor->GetVisited(), m_stmt_no, proc_name, m_name, is_uses);
  return GetPatternFormat();
}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_name == casted_other->m_name;
}

}
