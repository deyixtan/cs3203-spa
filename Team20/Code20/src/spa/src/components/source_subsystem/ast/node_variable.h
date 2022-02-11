#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_

#include "node_expression.h"

class VariableNode : public ExpressionNode {
 private:
  std::string m_name;

 public:
  VariableNode();
  VariableNode(std::string name);
  std::string getName();
  ExpressionType getExpressionType();
  std::string ToString(int _);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_
