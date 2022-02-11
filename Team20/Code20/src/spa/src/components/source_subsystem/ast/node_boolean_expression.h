#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_

#include "node_conditional_expression.h"

enum class BooleanOperator {
  AND,
  OR
};

class BooleanExpressionNode : public ConditionalExpressionNode {
 private:
  BooleanOperator m_op;
  std::shared_ptr<ConditionalExpressionNode> m_lhs;
  std::shared_ptr<ConditionalExpressionNode> m_rhs;

 public:
  BooleanExpressionNode(BooleanOperator op, std::shared_ptr<ConditionalExpressionNode> rhs);
  BooleanExpressionNode(BooleanOperator op,
                        std::shared_ptr<ConditionalExpressionNode> lhs,
                        std::shared_ptr<ConditionalExpressionNode> rhs);
  ConditionalType getConditionalType();
  void setLeft(std::shared_ptr<ConditionalExpressionNode> lhs);
  std::shared_ptr<ConditionalExpressionNode> getLHS();
  std::shared_ptr<ConditionalExpressionNode> getRHS();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_
