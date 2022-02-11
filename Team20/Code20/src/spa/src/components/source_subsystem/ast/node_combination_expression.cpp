#include "node_combination_expression.h"

CombinationExpressionNode::CombinationExpressionNode(Operation op, std::shared_ptr<ExpressionNode> right)
    : m_op(op), m_rhs(right), m_lhs(NULL) {}

CombinationExpressionNode::CombinationExpressionNode(Operation op,
                                                     std::shared_ptr<ExpressionNode> lhs,
                                                     std::shared_ptr<ExpressionNode> rhs)
    : m_op(op), m_rhs(rhs), m_lhs(lhs) {}

void CombinationExpressionNode::setLeftExpression(std::shared_ptr<ExpressionNode> left) {
  this->m_lhs = left;
}

std::shared_ptr<ExpressionNode> CombinationExpressionNode::getLHS() {
  return m_lhs;
}

std::shared_ptr<ExpressionNode> CombinationExpressionNode::getRHS() {
  return m_rhs;
}

Operation CombinationExpressionNode::getOperation() {
  return m_op;
}

std::string CombinationExpressionNode::getOperationLabel(Operation op) {
  switch (op) {
    case Operation::PLUS:return "+";
    case Operation::MINUS:return "-";
    case Operation::DIVIDE:return "/";
    case Operation::MULTIPLY:return "*";
    case Operation::MOD:return "%";
    default:return "";
  }
}

ExpressionType CombinationExpressionNode::getExpressionType() {
  return ExpressionType::COMBINATION;
}

std::string CombinationExpressionNode::ToString(int level) {
  return "(" + m_lhs->ToString(level) + " " + getOperationLabel(m_op) + " " + m_rhs->ToString(level) + ")";
}
