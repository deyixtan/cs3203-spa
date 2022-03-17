#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_

#include "node_expression.h"

class DesignExtractor;

class ConstantNode : public ExpressionNode {
 private:
  std::string m_value;

 public:
  explicit ConstantNode(std::string value);
  [[nodiscard]] std::string GetValue();
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
  std::string Accept(DesignExtractor *de, bool is_uses);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CONSTANT_H_
