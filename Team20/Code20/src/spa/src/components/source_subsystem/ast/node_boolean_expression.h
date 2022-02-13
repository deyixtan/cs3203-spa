#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_

#include "node_conditional_expression.h"

enum class BooleanOperator {
  AND,
  OR
};

class BooleanExpressionNode : public ConditionalExpressionNode {
 private:
  BooleanOperator m_boolean_operator;
  std::shared_ptr<ConditionalExpressionNode> m_left_expression;
  std::shared_ptr<ConditionalExpressionNode> m_right_expression;

 public:
  BooleanExpressionNode(BooleanOperator boolean_operator,
                        std::shared_ptr<ConditionalExpressionNode> left_expression,
                        std::shared_ptr<ConditionalExpressionNode> right_expression);
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetLeftExpression();
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetRightExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  [[nodiscard]] std::string ToString(int level) override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_
