#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_RELATIONAL_EXPRESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_RELATIONAL_EXPRESSION_H_

#include "node_combination_expression.h"
#include "node_conditional_expression.h"

enum class RelationalOperation {
  LESS_THAN,
  LESS_THAN_EQUALS,
  GREATER_THAN,
  GREATER_THAN_EQUALS,
  EQUALS,
  NOT_EQUALS
};

class RelationalExpressionNode : public ConditionalExpressionNode {
 private:
  RelationalOperation m_rop;
  std::shared_ptr<ExpressionNode> m_lhs;
  std::shared_ptr<ExpressionNode> m_rhs;

 public:
  RelationalExpressionNode(RelationalOperation op,
                           std::shared_ptr<ExpressionNode> lhs,
                           std::shared_ptr<ExpressionNode> rhs);
  ConditionalType getConditionalType();
  std::shared_ptr<ExpressionNode> getLHS();
  std::shared_ptr<ExpressionNode> getRHS();
  std::string getRelationalOpLabel(RelationalOperation op);
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_RELATIONAL_EXPRESSION_H_
