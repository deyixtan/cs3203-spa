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
  void Process(Populator populator, std::vector<std::string>* visited) override;
  //void Process(std::vector<std::string> *visited, std::string stmt, std::shared_ptr<ExpressionNode> expr) override;
  //std::string Process(std::vector<std::string> *visited, std::string stmt_num, std::shared_ptr<ExpressionNode> expr, int direction, std::string pattern) override;
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_
