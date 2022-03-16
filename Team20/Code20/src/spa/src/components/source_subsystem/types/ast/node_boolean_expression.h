#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_

#include "node_conditional_expression.h"
#include "components/source_subsystem/populator.h"

enum class BooleanOperator {
  AND,
  OR
};

class BooleanExpressionNode : public ConditionalExpressionNode {
 private:
  BooleanOperator m_boolean_operator;
  std::shared_ptr<ConditionalExpressionNode> m_left_expression;
  std::shared_ptr<ConditionalExpressionNode> m_right_expression;

 public:
  BooleanExpressionNode(BooleanOperator boolean_operator,
                        std::shared_ptr<ConditionalExpressionNode> left_expression,
                        std::shared_ptr<ConditionalExpressionNode> right_expression);
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetLeftExpression();
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> GetRightExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string>* visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_BOOLEAN_EXPRESSION_H_
