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

std::string BooleanExpressionNode::ToString() {
  return "(" + m_left_expression->ToString() + (m_boolean_operator == BooleanOperator::AND ? " && " : " || ")
      + m_right_expression->ToString() + ")";
}

std::string BooleanExpressionNode::GetPatternFormat() {
  return "";
}

bool BooleanExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const BooleanExpressionNode *>(&other);
  return m_boolean_operator == casted_other->m_boolean_operator
      && *m_left_expression == *(casted_other->m_left_expression)
      && *m_right_expression == *(casted_other->m_right_expression);
}

std::string BooleanExpressionNode::Process(Populator populator, std::vector<std::string>* visited, source::CfgGroupNode &cfg_node) {
  switch (m_boolean_operator) {
    case BooleanOperator::AND:
      return m_left_expression->Process(populator, visited, cfg_node) + "&&" + m_right_expression->Process(populator, visited, cfg_node);
    case BooleanOperator::OR:
      return m_left_expression->Process(populator, visited, cfg_node) + "||" + m_right_expression->Process(populator, visited, cfg_node);
  }
}
