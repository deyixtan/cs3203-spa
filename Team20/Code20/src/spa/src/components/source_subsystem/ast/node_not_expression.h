#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_

#include "node_conditional_expression.h"

class NotExpressionNode : public ConditionalExpressionNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_expr;

 public:
  NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expr);
  std::shared_ptr<ConditionalExpressionNode> getExpr();
  ConditionalType getConditionalType();
  std::string format(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_
