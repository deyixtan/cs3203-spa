#ifndef NODE_RELATIONAL_EXPRESSION_H
#define NODE_RELATIONAL_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"

namespace source {

enum class RelationOperator {
  EQUALS, NOT_EQUALS,
  GREATER_THAN_EQUALS, LESS_THAN_EQUALS,
  GREATER_THAN, LESS_THAN
};

class RelationalExpressionNode : public ConditionalExpressionNode {
 private:
  RelationOperator m_relation_operator;
  ExpressionNodePtr m_lhs;
  ExpressionNodePtr m_rhs;

 public:
  RelationalExpressionNode(RelationOperator relation_operator, ExpressionNodePtr lhs, ExpressionNodePtr rhs);
  [[nodiscard]] RelationOperator GetOperator();
  [[nodiscard]] static String GetOperatorLabel(RelationOperator relation_operator);
  [[nodiscard]] ExpressionNodePtr GetLhs();
  [[nodiscard]] ExpressionNodePtr GetRhs();
  void Accept(DesignExtractorPtr design_extractor) override;
  CfgNodePtr Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_RELATIONAL_EXPRESSION_H
