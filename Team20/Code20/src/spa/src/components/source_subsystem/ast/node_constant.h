#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_

#include "node_expression.h"

class ConstantNode : public ExpressionNode {
 private:
  std::string m_value;

 public:
  ConstantNode(std::string value);
  [[nodiscard]] std::string GetValue();
  [[nodiscard]] ExpressionType GetExpressionType() override;
  void Process(Populator populator, std::vector<std::string>* visited, std::string stmt) override;
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_
