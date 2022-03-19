#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_RELATIONAL_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_RELATIONAL_EXPRESSION_H_

#include "node_combination_expression.h"
#include "node_conditional_expression.h"
#include "components/source_subsystem/pkb_client.h"

class DesignExtractor;

enum class RelationOperator {
  LESS_THAN,
  LESS_THAN_EQUALS,
  GREATER_THAN,
  GREATER_THAN_EQUALS,
  EQUALS,
  NOT_EQUALS
};

class RelationalExpressionNode : public ConditionalExpressionNode {
 private:
  RelationOperator m_relation_operator;
  std::shared_ptr<ExpressionNode> m_left_expression;
  std::shared_ptr<ExpressionNode> m_right_expression;

 public:
  RelationalExpressionNode(RelationOperator relation_operator,
                           std::shared_ptr<ExpressionNode> left_expression,
                           std::shared_ptr<ExpressionNode> right_expression);
  [[nodiscard]] std::shared_ptr<ExpressionNode> GetLeftExpression();
  [[nodiscard]] std::shared_ptr<ExpressionNode> GetRightExpression();
  [[nodiscard]] std::string GetRelationOperatorLabel(RelationOperator relation_operator);
  [[nodiscard]] ConditionalType GetConditionalType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
  std::string Accept(DesignExtractor *de, std::string proc_name, bool is_uses);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_RELATIONAL_EXPRESSION_H_
