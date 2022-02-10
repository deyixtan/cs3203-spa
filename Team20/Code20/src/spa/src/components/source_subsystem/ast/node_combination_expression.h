#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_

#include "node_expression.h"

enum class Operation {
  PLUS,
  MINUS,
  DIVIDE,
  MULTIPLY,
  MOD,
};

class CombinationExpressionNode : public ExpressionNode {
 private:
  Operation m_op;
  std::shared_ptr<ExpressionNode> m_lhs;
  std::shared_ptr<ExpressionNode> m_rhs;

 public:
  CombinationExpressionNode(Operation op, std::shared_ptr<ExpressionNode> right);
  CombinationExpressionNode(Operation op, std::shared_ptr<ExpressionNode> lhs, std::shared_ptr<ExpressionNode> rhs);

  void setLeftExpression(std::shared_ptr<ExpressionNode> left);
  std::shared_ptr<ExpressionNode> getLHS();
  std::shared_ptr<ExpressionNode> getRHS();
  Operation getOperation();
  std::string getOperationLabel(Operation op);
  ExpressionType getExpressionType();
  std::string format(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_COMBINATION_EXPRESSION_H_
