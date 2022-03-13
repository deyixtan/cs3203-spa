#include "node_constant.h"

ConstantNode::ConstantNode(std::string value) : m_value(value) {}

std::string ConstantNode::GetValue() {
  return m_value;
}

ExpressionType ConstantNode::GetExpressionType() {
  return ExpressionType::CONSTANT;
}

std::string ConstantNode::ToString() {
  return m_value;
}

void ConstantNode::Process(Populator populator, std::vector<std::string> *visited) {
  populator.PopulateConst(m_value);
}

std::string ConstantNode::GetPatternFormat() {
  return "(" + m_value + ")";
}

bool ConstantNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const ConstantNode *>(&other);
  return m_value == casted_other->m_value;
}
