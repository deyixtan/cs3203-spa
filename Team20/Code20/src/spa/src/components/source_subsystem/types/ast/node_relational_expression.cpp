#include "node_relational_expression.h"
#include "../../iterator/design_extractor.h"
#include "node_combination_expression.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

RelationalExpressionNode::RelationalExpressionNode(RelationOperator relation_operator,
                                                   ExpressionNodePtr left_expression,
                                                   ExpressionNodePtr right_expression)
    : m_relation_operator(relation_operator),
      m_left_expression(left_expression),
      m_right_expression(right_expression) {}

ExpressionNodePtr RelationalExpressionNode::GetLeftExpression() {
  return m_left_expression;
}

ExpressionNodePtr RelationalExpressionNode::GetRightExpression() {
  return m_right_expression;
}

String RelationalExpressionNode::GetRelationOperatorLabel(RelationOperator relation_operator) {
  switch (relation_operator) {
    case RelationOperator::LESS_THAN:return "<";
    case RelationOperator::LESS_THAN_EQUALS:return "<=";
    case RelationOperator::GREATER_THAN:return ">";
    case RelationOperator::GREATER_THAN_EQUALS:return ">=";
    case RelationOperator::EQUALS:return "==";
    case RelationOperator::NOT_EQUALS:return "!=";
    default:return "";
  }
}

ConditionalType RelationalExpressionNode::GetConditionalType() {
  return ConditionalType::RELATIONAL;
}

String RelationalExpressionNode::GetPatternFormat() {
  return "";
}

String RelationalExpressionNode::Accept(DesignExtractor *de, String proc_name, bool is_uses) {
  return "(" + de->Visit(m_left_expression, proc_name, is_uses) + GetRelationOperatorLabel(m_relation_operator)
      + de->Visit(m_right_expression, proc_name, is_uses) + ")";
}

bool RelationalExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const RelationalExpressionNode *>(&other);
  return m_relation_operator == casted_other->m_relation_operator
      && *m_left_expression == *(casted_other->m_left_expression)
      && *m_right_expression == *(casted_other->m_right_expression);
}

}
