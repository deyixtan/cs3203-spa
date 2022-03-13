#include "node_not_expression.h"

NotExpressionNode::NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expression)
    : m_expression(expression) {}

std::shared_ptr<ConditionalExpressionNode> NotExpressionNode::GetExpression() {
  return m_expression;
}

ConditionalType NotExpressionNode::GetConditionalType() {
  return ConditionalType::NOT;
}

std::string NotExpressionNode::ToString() {
  return "(!" + m_expression->ToString() + ")";
}

std::string NotExpressionNode::GetPatternFormat() {
  return "";
}

bool NotExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const NotExpressionNode *>(&other);
  return *m_expression == *(casted_other->m_expression);
}

std::string NotExpressionNode::Process(Populator populator, std::vector<std::string>* visited, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  return m_expression->Process(populator, visited, cfg_node);
}
