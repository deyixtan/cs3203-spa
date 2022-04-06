#include "node_not_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

NotExpressionNode::NotExpressionNode(ConditionalExpressionNodePtr expression)
    : m_expression(expression) {}

ConditionalExpressionNodePtr NotExpressionNode::GetExpression() {
  return m_expression;
}

void NotExpressionNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<NotExpressionNode>(shared_from_this()));
}

bool NotExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const NotExpressionNode *>(&other);
  return *m_expression == *(casted_other->m_expression);
}

}
