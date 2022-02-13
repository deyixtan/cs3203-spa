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
