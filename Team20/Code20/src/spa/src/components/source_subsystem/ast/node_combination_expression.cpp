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

ExpressionType CombinationExpressionNode::GetExpressionType() {
  return ExpressionType::COMBINATION;
}

std::string CombinationExpressionNode::Process(Populator populator, std::vector<std::string> *visited, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  return "(" + m_left_expression->Process(populator, visited, cfg_node) + GetArithmeticOperatorLabel(m_arithmetic_operator)
      + m_right_expression->Process(populator, visited, cfg_node) + ")";
}

std::string CombinationExpressionNode::ToString() {
  return "(" + m_left_expression->ToString() + " " + GetArithmeticOperatorLabel(m_arithmetic_operator) + " "
      + m_right_expression->ToString() + ")";
}

std::string CombinationExpressionNode::GetPatternFormat() {
  return "(" + m_left_expression->GetPatternFormat() + GetArithmeticOperatorLabel(m_arithmetic_operator)
      + m_right_expression->GetPatternFormat() + ")";
}

bool CombinationExpressionNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const CombinationExpressionNode *>(&other);
  return m_arithmetic_operator == casted_other->m_arithmetic_operator
      && *m_left_expression == *(casted_other->m_left_expression)
      && *m_right_expression == *(casted_other->m_right_expression);
}
