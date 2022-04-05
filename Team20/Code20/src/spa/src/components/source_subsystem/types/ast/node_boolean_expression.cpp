#include "node_boolean_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

BooleanExpressionNode::BooleanExpressionNode(BooleanOperator boolean_operator,
                                             ConditionalExpressionNodePtr left_expression,
                                             ConditionalExpressionNodePtr right_expression)
    : m_boolean_operator(boolean_operator), m_left_expression(left_expression), m_right_expression(right_expression) {}

ConditionalExpressionNodePtr BooleanExpressionNode::GetLeftExpression() {
  return m_left_expression;
}

ConditionalExpressionNodePtr BooleanExpressionNode::GetRightExpression() {
  return m_right_expression;
}

ConditionalType BooleanExpressionNode::GetConditionalType() {
  return ConditionalType::BOOLEAN;
}

String BooleanExpressionNode::Accept(DesignExtractor *de, String proc_name, bool is_uses) {
  String lhs = de->Visit(m_left_expression, proc_name, is_uses);
  String rhs = de->Visit(m_right_expression, proc_name, is_uses);
  if (m_boolean_operator == BooleanOperator::AND) {
    return lhs + "&&" + rhs;
  }
  return lhs + "||" + rhs;
}

bool BooleanExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const BooleanExpressionNode *>(&other);
  return m_boolean_operator == casted_other->m_boolean_operator
      && *m_left_expression == *(casted_other->m_left_expression)
      && *m_right_expression == *(casted_other->m_right_expression);
}

}
