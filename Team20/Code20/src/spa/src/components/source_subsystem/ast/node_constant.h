#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_

#include "node_expression.h"

class ConstantNode : public ExpressionNode
{
 private:
  std::string m_value;

 public:
  ConstantNode(std::string value);
  std::string getValue();
  ExpressionType getExpressionType();
  std::string ToString(int lvl);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_
