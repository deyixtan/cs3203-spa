#include "node_variable.h"

VariableNode::VariableNode() : m_identifier("") {}

VariableNode::VariableNode(std::string identifier) : m_identifier(identifier) {}

std::string VariableNode::GetIdentifier() {
  return m_identifier;
}

ExpressionType VariableNode::GetExpressionType() {
  return ExpressionType::VARIABLE;
}

std::string VariableNode::ToString(int level) {
  return "$" + m_identifier;
}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_identifier == casted_other->m_identifier;
}

void VariableNode::Process(Populator populator, std::vector<std::string>* visited, std::string stmt) {
  std::string var_name = m_identifier;
  for (std::string s : *visited) {
    populator.PopulateUses(s, var_name);
  }
  populator.PopulateUses(stmt, var_name);
  populator.PopulateVars(var_name);
}
