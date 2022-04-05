#ifndef NODE_NOT_EXPRESSION_H
#define NODE_NOT_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "node_conditional_expression.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

class NotExpressionNode : public ConditionalExpressionNode {
 private:
  ConditionalExpressionNodePtr m_expression;

 public:
  NotExpressionNode(ConditionalExpressionNodePtr expression);
  [[nodiscard]] ConditionalExpressionNodePtr GetExpression();
  [[nodiscard]] ConditionalType GetConditionalType() override;
  [[nodiscard]] String GetPatternFormat() override;
  String Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_NOT_EXPRESSION_H
