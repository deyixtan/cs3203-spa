#include "node_variable.h"

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

std::string VariableNode::Process(Populator populator, std::vector<std::string>* visited) {
  std::string var_name = m_identifier;
  for (std::string s : *visited) {
    populator.PopulateUses(s, var_name);
  }
  populator.PopulateUses(m_stmt, var_name);
  populator.PopulateVars(var_name);
  return GetPatternFormat();
}
