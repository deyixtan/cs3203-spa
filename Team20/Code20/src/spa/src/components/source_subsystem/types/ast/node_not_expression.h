#ifndef NODE_NOT_EXPRESSION_H
#define NODE_NOT_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"

namespace source {

class NotExpressionNode : public ConditionalExpressionNode {
 private:
  ConditionalExpressionNodePtr m_expression;

 public:
  NotExpressionNode(ConditionalExpressionNodePtr expression);
  [[nodiscard]] ConditionalExpressionNodePtr GetExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  String Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_NOT_EXPRESSION_H
