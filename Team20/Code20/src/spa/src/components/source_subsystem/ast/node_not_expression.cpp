#include "node_not_expression.h"

NotExpressionNode::NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expression)
    : m_expression(expression) {}

std::shared_ptr<ConditionalExpressionNode> NotExpressionNode::GetExpression() {
  return m_expression;
}

ConditionalType NotExpressionNode::GetConditionalType() {
  return ConditionalType::NOT;
}

std::string NotExpressionNode::ToString(int level) {
  return "(!" + m_expression->ToString(level) + ")";
}
