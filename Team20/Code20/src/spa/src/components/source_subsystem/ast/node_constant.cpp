#include "node_constant.h"

ConstantNode::ConstantNode(std::string value) : m_value(value) {}

std::string ConstantNode::GetValue() {
  return m_value;
}

ExpressionType ConstantNode::GetExpressionType() {
  return ExpressionType::CONSTANT;
}

std::string ConstantNode::ToString(int level) {
  return m_value;
}

bool ConstantNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const ConstantNode *>(&other);
  return m_value == casted_other->m_value;
}
