#include "node_variable.h"
#include "../../iterator/design_extractor.h"

namespace source {

VariableNode::VariableNode() : m_identifier("") {}

VariableNode::VariableNode(String identifier, String stmt) : m_identifier(identifier), m_stmt(stmt) {}

String VariableNode::GetIdentifier() {
  return m_identifier;
}

ExpressionType VariableNode::GetExpressionType() {
  return ExpressionType::VARIABLE;
}

String VariableNode::GetPatternFormat() {
  return "(" + m_identifier + ")";
}

String VariableNode::Accept(DesignExtractor *de, String proc_name, bool is_uses) {
  de->GetPkbClient()->PopulateVars(de->GetVisited(), m_stmt, proc_name, m_identifier, is_uses);
  return GetPatternFormat();
}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_identifier == casted_other->m_identifier;
}

}
