#include "node_combination_expression.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

CombinationExpressionNode::CombinationExpressionNode(ArithmeticOperator arithmetic_operator, ExpressionNodePtr rhs)
    : m_arithmetic_operator(arithmetic_operator), m_lhs(nullptr), m_rhs(std::move(rhs)) {}

CombinationExpressionNode::CombinationExpressionNode(ArithmeticOperator arithmetic_operator, ExpressionNodePtr lhs, ExpressionNodePtr rhs)
    : m_arithmetic_operator(arithmetic_operator), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

ArithmeticOperator CombinationExpressionNode::GetOperator() {
  return m_arithmetic_operator;
}

String CombinationExpressionNode::GetOperatorLabel(ArithmeticOperator arithmetic_operator) {
  switch (arithmetic_operator) {
    case ArithmeticOperator::PLUS:return "+";
    case ArithmeticOperator::MINUS:return "-";
    case ArithmeticOperator::DIVIDE:return "/";
    case ArithmeticOperator::MULTIPLY:return "*";
    case ArithmeticOperator::MOD:return "%";
    default:return "";
  }
}

ExpressionNodePtr CombinationExpressionNode::GetLhs() {
  return m_lhs;
}

ExpressionNodePtr CombinationExpressionNode::GetRhs() {
  return m_rhs;
}

String CombinationExpressionNode::GetPatternFormat() {
  return "(" + m_lhs->GetPatternFormat() + GetOperatorLabel(m_arithmetic_operator) + m_rhs->GetPatternFormat() + ")";
}

void CombinationExpressionNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<CombinationExpressionNode>(shared_from_this()));
}

CfgNodePtr CombinationExpressionNode::Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) {}

bool CombinationExpressionNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const CombinationExpressionNode *>(&other);
  return m_arithmetic_operator == casted_other->m_arithmetic_operator
      && *m_lhs == *(casted_other->m_lhs)
      && *m_rhs == *(casted_other->m_rhs);
}

}
