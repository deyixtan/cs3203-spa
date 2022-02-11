#include "node_boolean_expression.h"

BooleanExpressionNode::BooleanExpressionNode(BooleanOperator op,
                                             std::shared_ptr<ConditionalExpressionNode> lhs,
                                             std::shared_ptr<ConditionalExpressionNode> rhs)
    : m_op(op), m_lhs(lhs), m_rhs(rhs) {}

BooleanExpressionNode::BooleanExpressionNode(BooleanOperator op, std::shared_ptr<ConditionalExpressionNode> rhs) : m_op(
    op), m_lhs(NULL), m_rhs(rhs) {}

ConditionalType BooleanExpressionNode::getConditionalType() {
  return ConditionalType::BOOLEAN;
}

void BooleanExpressionNode::setLeft(std::shared_ptr<ConditionalExpressionNode> lhs) {
  this->m_lhs = lhs;
}

std::shared_ptr<ConditionalExpressionNode> BooleanExpressionNode::getLHS() {
  return m_lhs;
}

std::shared_ptr<ConditionalExpressionNode> BooleanExpressionNode::getRHS() {
  return m_rhs;
}

std::string BooleanExpressionNode::ToString(int level) {
  return "(" + m_lhs->ToString(level) + (m_op == BooleanOperator::AND ? " && " : " || ") + m_rhs->ToString(level) + ")";
}
