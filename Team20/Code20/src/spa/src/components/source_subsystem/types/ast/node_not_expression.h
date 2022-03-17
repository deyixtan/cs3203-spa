#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_

#include "node_conditional_expression.h"
#include "components/source_subsystem/populator.h"

class DesignExtractor;

class NotExpressionNode : public ConditionalExpressionNode {
 private:
  std::shared_ptr<ConditionalExpressionNode> m_expression;

 public:
  NotExpressionNode(std::shared_ptr<ConditionalExpressionNode> expression);
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
  std::string Accept(DesignExtractor *de, bool is_uses);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_NOT_EXPRESSION_H_
