#include "node_boolean_expression.h"

BooleanExpressionNode::BooleanExpressionNode(BooleanOperator boolean_operator,
                                             std::shared_ptr<ConditionalExpressionNode> left_expression,
                                             std::shared_ptr<ConditionalExpressionNode> right_expression)
    : m_boolean_operator(boolean_operator), m_left_expression(left_expression), m_right_expression(right_expression) {}

std::shared_ptr<ConditionalExpressionNode> BooleanExpressionNode::GetLeftExpression() {
  return m_left_expression;
}

std::shared_ptr<ConditionalExpressionNode> BooleanExpressionNode::GetRightExpression() {
  return m_right_expression;
}

ConditionalType BooleanExpressionNode::GetConditionalType() {
  return ConditionalType::BOOLEAN;
}

std::string BooleanExpressionNode::ToString(int level) {
  return "(" + m_left_expression->ToString(level) + (m_boolean_operator == BooleanOperator::AND ? " && " : " || ")
      + m_right_expression->ToString(level) + ")";
}

bool BooleanExpressionNode::operator==(const BooleanExpressionNode &other) const {
  return m_boolean_operator == other.m_boolean_operator && m_left_expression == other.m_left_expression
      && m_right_expression == other.m_right_expression;
}
