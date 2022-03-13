#include "node_relational_expression.h"

RelationalExpressionNode::RelationalExpressionNode(RelationOperator relation_operator,
                                                   std::shared_ptr<ExpressionNode> left_expression,
                                                   std::shared_ptr<ExpressionNode> right_expression)
    : m_relation_operator(relation_operator),
      m_left_expression(left_expression),
      m_right_expression(right_expression) {}

std::shared_ptr<ExpressionNode> RelationalExpressionNode::GetLeftExpression() {
  return m_left_expression;
}

std::shared_ptr<ExpressionNode> RelationalExpressionNode::GetRightExpression() {
  return m_right_expression;
}

std::string RelationalExpressionNode::GetRelationOperatorLabel(RelationOperator relation_operator) {
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

std::string RelationalExpressionNode::ToString() {
  return "(" + m_left_expression->ToString() + " " + GetRelationOperatorLabel(m_relation_operator) + " "
      + m_right_expression->ToString() + ")";
}

std::string RelationalExpressionNode::GetPatternFormat() {
  return "";
}

bool RelationalExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const RelationalExpressionNode *>(&other);
  return m_relation_operator == casted_other->m_relation_operator
      && *m_left_expression == *(casted_other->m_left_expression)
      && *m_right_expression == *(casted_other->m_right_expression);
}

std::string RelationalExpressionNode::Process(Populator populator, std::vector<std::string>* visited) {
  switch (m_relation_operator) {
    case RelationOperator::LESS_THAN:
      return m_left_expression->Process(populator, visited) + "<" + m_right_expression->Process(populator, visited);
    case RelationOperator::LESS_THAN_EQUALS:
      return m_left_expression->Process(populator, visited) + "<=" + m_right_expression->Process(populator, visited);
    case RelationOperator::GREATER_THAN:
      return m_left_expression->Process(populator, visited) + ">" + m_right_expression->Process(populator, visited);
    case RelationOperator::GREATER_THAN_EQUALS:
      return m_left_expression->Process(populator, visited) + ">=" + m_right_expression->Process(populator, visited);
    case RelationOperator::EQUALS:
      return m_left_expression->Process(populator, visited) + "==" + m_right_expression->Process(populator, visited);
    case RelationOperator::NOT_EQUALS:
      return m_left_expression->Process(populator, visited) + "!=" + m_right_expression->Process(populator, visited);
  }
}
