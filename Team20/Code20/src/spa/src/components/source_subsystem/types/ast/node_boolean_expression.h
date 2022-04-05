#ifndef NODE_BOOLEAN_EXPRESSION_H
#define NODE_BOOLEAN_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"

namespace source {

enum class BooleanOperator {
  AND, OR
};

class BooleanExpressionNode : public ConditionalExpressionNode {
 private:
  BooleanOperator m_boolean_operator;
  ConditionalExpressionNodePtr m_left_expression;
  ConditionalExpressionNodePtr m_right_expression;

 public:
  BooleanExpressionNode(BooleanOperator boolean_operator,
                        ConditionalExpressionNodePtr left_expression,
                        ConditionalExpressionNodePtr right_expression);
  [[nodiscard]] ConditionalExpressionNodePtr GetLeftExpression();
  [[nodiscard]] ConditionalExpressionNodePtr GetRightExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  String Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_BOOLEAN_EXPRESSION_H
