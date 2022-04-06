#include "node_relational_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_expression.h"

namespace source {

RelationalExpressionNode::RelationalExpressionNode(RelationOperator relation_operator,
                                                   ExpressionNodePtr lhs,
                                                   ExpressionNodePtr rhs)
    : m_relation_operator(relation_operator), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

RelationOperator RelationalExpressionNode::GetOperator() {
  return m_relation_operator;
}

String RelationalExpressionNode::GetOperatorLabel(RelationOperator relation_operator) {
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

ExpressionNodePtr RelationalExpressionNode::GetLhs() {
  return m_lhs;
}

ExpressionNodePtr RelationalExpressionNode::GetRhs() {
  return m_rhs;
}

void RelationalExpressionNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<RelationalExpressionNode>(shared_from_this()));
}

CfgNodePtr RelationalExpressionNode::Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) {}

bool RelationalExpressionNode::operator==(const ConditionalExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const RelationalExpressionNode *>(&other);
  return m_relation_operator == casted_other->m_relation_operator
      && *m_lhs == *(casted_other->m_lhs)
      && *m_rhs == *(casted_other->m_rhs);
}

}
