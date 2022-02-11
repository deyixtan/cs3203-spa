#include "node_constant.h"

ConstantNode::ConstantNode(std::string value) : m_value(value) {}

std::string ConstantNode::getValue() {
  return m_value;
}

ExpressionType ConstantNode::getExpressionType() {
  return ExpressionType::CONSTANT;
}

std::string ConstantNode::ToString(int _) {
  return m_value;
}
