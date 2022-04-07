#ifndef NODE_BOOLEAN_EXPRESSION_H
#define NODE_BOOLEAN_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"

namespace source {

enum class BooleanOperator {
  AND, OR
};

class BooleanExpressionNode : public ConditionalExpressionNode {
 private:
  BooleanOperator m_boolean_operator;
  ConditionalExpressionNodePtr m_lhs;
  ConditionalExpressionNodePtr m_rhs;

 public:
  BooleanExpressionNode(BooleanOperator boolean_operator,
                        ConditionalExpressionNodePtr lhs,
                        ConditionalExpressionNodePtr rhs);
  [[nodiscard]] BooleanOperator GetOperator();
  [[nodiscard]] ConditionalExpressionNodePtr GetLhs();
  [[nodiscard]] ConditionalExpressionNodePtr GetRhs();
  void Accept(DesignExtractorPtr design_extractor) override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_BOOLEAN_EXPRESSION_H
