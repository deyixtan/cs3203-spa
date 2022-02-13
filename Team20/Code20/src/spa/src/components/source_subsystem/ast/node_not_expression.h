#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_

#include "node_conditional_expression.h"

class NotExpressionNode : public ConditionalExpressionNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_expression;

 public:
  NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expression);
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_
