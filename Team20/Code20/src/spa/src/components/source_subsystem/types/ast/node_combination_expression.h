#ifndef NODE_COMBINATION_EXPRESSION_H
#define NODE_COMBINATION_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_expression.h"

namespace source {

enum class ArithmeticOperator {
  PLUS, MINUS, MULTIPLY, DIVIDE, MOD
};

class CombinationExpressionNode : public ExpressionNode {
 private:
  ArithmeticOperator m_arithmetic_operator;
  ExpressionNodePtr m_lhs;
  ExpressionNodePtr m_rhs;

 public:
  CombinationExpressionNode(ArithmeticOperator arithmetic_operator, ExpressionNodePtr rhs);
  CombinationExpressionNode(ArithmeticOperator arithmetic_operator, ExpressionNodePtr lhs, ExpressionNodePtr rhs);
  [[nodiscard]] ArithmeticOperator GetOperator();
  [[nodiscard]] static String GetOperatorLabel(ArithmeticOperator arithmetic_operator);
  [[nodiscard]] ExpressionNodePtr GetLhs();
  [[nodiscard]] ExpressionNodePtr GetRhs();
  [[nodiscard]] String GetPatternFormat() override;
  void Accept(DesignExtractorPtr design_extractor) override;
  void Accept(CfgBuilderPtr cfg_builder) override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

}

#endif //NODE_COMBINATION_EXPRESSION_H
