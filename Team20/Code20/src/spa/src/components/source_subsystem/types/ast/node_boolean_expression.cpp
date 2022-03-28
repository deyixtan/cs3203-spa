#include "node_boolean_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"

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

std::string BooleanExpressionNode::Accept(DesignExtractor *de, std::string proc_name, bool is_uses) {
  std::string lhs = de->Visit(m_left_expression, proc_name, is_uses);
  std::string rhs = de->Visit(m_right_expression, proc_name, is_uses);
  if (m_boolean_operator == BooleanOperator::AND) {
    return lhs + "&&" + rhs;
  }
  return lhs + "||" + rhs;
}
