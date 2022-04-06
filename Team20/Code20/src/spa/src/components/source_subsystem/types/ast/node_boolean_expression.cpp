#include "node_boolean_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

BooleanExpressionNode::BooleanExpressionNode(BooleanOperator boolean_operator,
                                             ConditionalExpressionNodePtr lhs,
                                             ConditionalExpressionNodePtr rhs)
    : m_boolean_operator(boolean_operator), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

BooleanOperator BooleanExpressionNode::GetOperator() {
  return m_boolean_operator;
}

ConditionalExpressionNodePtr BooleanExpressionNode::GetLhs() {
  return m_lhs;
}

ConditionalExpressionNodePtr BooleanExpressionNode::GetRhs() {
  return m_rhs;
}

void BooleanExpressionNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<BooleanExpressionNode>(shared_from_this()));
}

bool BooleanExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const BooleanExpressionNode *>(&other);
  return m_boolean_operator == casted_other->m_boolean_operator
      && *m_lhs == *(casted_other->m_lhs)
      && *m_rhs == *(casted_other->m_rhs);
}

}
