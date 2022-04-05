#include "node_variable.h"
#include "../../iterator/design_extractor.h"

namespace source {

VariableNode::VariableNode() : m_identifier("") {}

VariableNode::VariableNode(std::string identifier, std::string stmt) : m_identifier(identifier), m_stmt(stmt) {}

std::string VariableNode::GetIdentifier() {
  return m_identifier;
}

ExpressionType VariableNode::GetExpressionType() {
  return ExpressionType::VARIABLE;
}

std::string VariableNode::ToString() {
  return "$" + m_identifier;
}

std::string VariableNode::GetPatternFormat() {
  return "(" + m_identifier + ")";
}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_identifier == casted_other->m_identifier;
}

std::string VariableNode::Accept(DesignExtractor *de, std::string proc_name, bool is_uses) {
  de->GetPkbClient()->PopulateVars(de->GetVisited(), m_stmt, proc_name, m_identifier, is_uses);
  return GetPatternFormat();
}

}
