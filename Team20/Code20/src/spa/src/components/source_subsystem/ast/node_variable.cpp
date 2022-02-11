#include "node_variable.h"

VariableNode::VariableNode() : m_name("") {}

VariableNode::VariableNode(std::string name) : m_name(name) {}

std::string VariableNode::getName() {
  return m_name;
}

ExpressionType VariableNode::getExpressionType() {
  return ExpressionType::VARIABLE;
}

std::string VariableNode::ToString(int _) {
  return "$" + m_name;
}
