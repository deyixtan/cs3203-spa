#ifndef NODE_NOT_EXPRESSION_H
#define NODE_NOT_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"

namespace source {

class NotExpressionNode : public ConditionalExpressionNode, std::enable_shared_from_this<NotExpressionNode> {
 private:
  ConditionalExpressionNodePtr m_expression;

 public:
  NotExpressionNode(ConditionalExpressionNodePtr expression);
  [[nodiscard]] ConditionalExpressionNodePtr GetExpression();
  void Accept(DesignExtractorPtr design_extractor) override;
  [[nodiscard]] bool operator==(const ConditionalExpressionNode &other) const override;
};

}

#endif //NODE_NOT_EXPRESSION_H
