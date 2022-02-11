#include "node_not_expression.h"
NotExpressionNode::NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expr) : m_expr(expr) {}

std::shared_ptr<ConditionalExpressionNode> NotExpressionNode::getExpr() {
  return m_expr;
}

ConditionalType NotExpressionNode::getConditionalType() {
  return ConditionalType::NOT;
}

std::string NotExpressionNode::ToString(int level) {
  return "(!" + m_expr->ToString(level) + ")";
}
