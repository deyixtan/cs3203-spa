#ifndef NODE_RELATIONAL_EXPRESSION_H
#define NODE_RELATIONAL_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "node_conditional_expression.h"

namespace source {

enum class RelationOperator {
  EQUALS, NOT_EQUALS,
  GREATER_THAN_EQUALS, LESS_THAN_EQUALS,
  GREATER_THAN, LESS_THAN
};

class RelationalExpressionNode : public ConditionalExpressionNode {
 private:
  RelationOperator m_relation_operator;
  ExpressionNodePtr m_left_expression;
  ExpressionNodePtr m_right_expression;

 public:
  RelationalExpressionNode(RelationOperator relation_operator,
                           ExpressionNodePtr left_expression,
                           ExpressionNodePtr right_expression);
  [[nodiscard]] ExpressionNodePtr GetLeftExpression();
  [[nodiscard]] ExpressionNodePtr GetRightExpression();
  [[nodiscard]] String GetRelationOperatorLabel(RelationOperator relation_operator);
  [[nodiscard]] ConditionalType GetConditionalType() override;
  String Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_RELATIONAL_EXPRESSION_H
