#include "node_relational_expression.h"

RelationalExpressionNode::RelationalExpressionNode(RelationalOperation op,
                                                   std::shared_ptr<ExpressionNode> lhs,
                                                   std::shared_ptr<ExpressionNode> rhs)
    : m_rop(op), m_lhs(lhs), m_rhs(rhs) {}

ConditionalType RelationalExpressionNode::getConditionalType() {
  return ConditionalType::RELATIONAL;
}

std::shared_ptr<ExpressionNode> RelationalExpressionNode::getLHS() {
  return m_lhs;
}

std::shared_ptr<ExpressionNode> RelationalExpressionNode::getRHS() {
  return m_rhs;
}

std::string RelationalExpressionNode::getRelationalOpLabel(RelationalOperation op) {
  switch (op) {
    case RelationalOperation::LESS_THAN:return "<";
    case RelationalOperation::LESS_THAN_EQUALS:return "<=";
    case RelationalOperation::GREATER_THAN:return ">";
    case RelationalOperation::GREATER_THAN_EQUALS:return ">=";
    case RelationalOperation::EQUALS:return "==";
    case RelationalOperation::NOT_EQUALS:return "!=";
    default:return "";
  }
}

std::string RelationalExpressionNode::format(int level) {
  return "(" + m_lhs->format(level) + " " + getRelationalOpLabel(m_rop) + " " + m_rhs->format(level) + ")";
}
