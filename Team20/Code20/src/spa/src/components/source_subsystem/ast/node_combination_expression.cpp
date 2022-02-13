#include "node_combination_expression.h"

CombinationExpressionNode::CombinationExpressionNode(ArithmeticOperator arithmetic_operator,
                                                     std::shared_ptr<ExpressionNode> right_expression)
    : m_arithmetic_operator(arithmetic_operator), m_left_expression(NULL), m_right_expression(right_expression) {}

CombinationExpressionNode::CombinationExpressionNode(ArithmeticOperator arithmetic_operator,
                                                     std::shared_ptr<ExpressionNode> left_expression,
                                                     std::shared_ptr<ExpressionNode> right_expression)
    : m_arithmetic_operator(arithmetic_operator),
      m_left_expression(left_expression),
      m_right_expression(right_expression) {}

std::shared_ptr<ExpressionNode> CombinationExpressionNode::GetLeftExpression() {
  return m_left_expression;
}

std::shared_ptr<ExpressionNode> CombinationExpressionNode::GetRightExpression() {
  return m_right_expression;
}

ArithmeticOperator CombinationExpressionNode::GetArithmeticOperator() {
  return m_arithmetic_operator;
}

std::string CombinationExpressionNode::GetArithmeticOperatorLabel(ArithmeticOperator arithmetic_operator) {
  switch (arithmetic_operator) {
    case ArithmeticOperator::PLUS:return "+";
    case ArithmeticOperator::MINUS:return "-";
    case ArithmeticOperator::DIVIDE:return "/";
    case ArithmeticOperator::MULTIPLY:return "*";
    case ArithmeticOperator::MOD:return "%";
    default:return "";
  }
}

void CombinationExpressionNode::SetLeftExpression(std::shared_ptr<ExpressionNode> left_expression) {
  m_left_expression = left_expression;
}

ExpressionType CombinationExpressionNode::GetExpressionType() {
  return ExpressionType::COMBINATION;
}

std::string CombinationExpressionNode::ToString(int level) {
  return "(" + m_left_expression->ToString(level) + " " + GetArithmeticOperatorLabel(m_arithmetic_operator) + " "
      + m_right_expression->ToString(level) + ")";
}