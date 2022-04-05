#include "node_not_expression.h"
#include "../../iterator/design_extractor.h"

namespace source {

NotExpressionNode::NotExpressionNode(ConditionalExpressionNodePtr expression)
    : m_expression(expression) {}

ConditionalExpressionNodePtr NotExpressionNode::GetExpression() {
  return m_expression;
}

ConditionalType NotExpressionNode::GetConditionalType() {
  return ConditionalType::NOT;
}

String NotExpressionNode::GetPatternFormat() {
  return "";
}

String NotExpressionNode::Accept(DesignExtractor *de, String proc_name, bool is_uses) {
  return de->Visit(m_expression, proc_name, is_uses);
}

bool NotExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const NotExpressionNode *>(&other);
  return *m_expression == *(casted_other->m_expression);
}

}
