#include "node_not_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

NotExpressionNode::NotExpressionNode(ConditionalExpressionNodePtr expression)
    : m_expression(expression) {}

ConditionalExpressionNodePtr NotExpressionNode::GetExpression() {
  return m_expression;
}

void NotExpressionNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(this);
}

//
//String NotExpressionNode::Accept(DesignExtractor *design_extractor, String proc_name, bool is_uses) {
//  return design_extractor->Visit(m_expression, proc_name, is_uses);
//}

bool NotExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const NotExpressionNode *>(&other);
  return *m_expression == *(casted_other->m_expression);
}

}
