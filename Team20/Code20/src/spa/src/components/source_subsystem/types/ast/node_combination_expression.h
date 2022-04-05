#ifndef NODE_COMBINATION_EXPRESSION_H
#define NODE_COMBINATION_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "node_expression.h"

namespace source {

enum class ArithmeticOperator {
  PLUS, MINUS, MULTIPLY, DIVIDE, MOD
};

class DesignExtractor;

class CombinationExpressionNode : public ExpressionNode {
 private:
  ArithmeticOperator m_arithmetic_operator;
  ExpressionNodePtr m_left_expression;
  ExpressionNodePtr m_right_expression;

 public:
  CombinationExpressionNode(ArithmeticOperator arithmetic_operator, ExpressionNodePtr right_expression);
  CombinationExpressionNode(ArithmeticOperator arithmetic_operator,
                            ExpressionNodePtr left_expression,
                            ExpressionNodePtr right_expression);
  [[nodiscard]] ExpressionNodePtr GetLeftExpression();
  [[nodiscard]] ExpressionNodePtr GetRightExpression();
  [[nodiscard]] ArithmeticOperator GetArithmeticOperator();
  [[nodiscard]] String GetArithmeticOperatorLabel(ArithmeticOperator arithmetic_operator);
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] String GetPatternFormat();
  String Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

}

#endif //NODE_COMBINATION_EXPRESSION_H
