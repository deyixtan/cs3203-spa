#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_

#include "node_expression.h"

enum class ArithmeticOperator {
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  MOD,
};

class DesignExtractor;

class CombinationExpressionNode : public ExpressionNode {
 private:
  ArithmeticOperator m_arithmetic_operator;
  std::shared_ptr<ExpressionNode> m_left_expression;
  std::shared_ptr<ExpressionNode> m_right_expression;

 public:
  CombinationExpressionNode(ArithmeticOperator arithmetic_operator, std::shared_ptr<ExpressionNode> right_expression);
  CombinationExpressionNode(ArithmeticOperator arithmetic_operator,
                            std::shared_ptr<ExpressionNode> left_expression,
                            std::shared_ptr<ExpressionNode> right_expression);
  [[nodiscard]] std::shared_ptr<ExpressionNode> GetLeftExpression();
  [[nodiscard]] std::shared_ptr<ExpressionNode> GetRightExpression();
  [[nodiscard]] ArithmeticOperator GetArithmeticOperator();
  [[nodiscard]] std::string GetArithmeticOperatorLabel(ArithmeticOperator arithmetic_operator);
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat();
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
  std::string Accept(DesignExtractor *de, bool is_uses);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_
