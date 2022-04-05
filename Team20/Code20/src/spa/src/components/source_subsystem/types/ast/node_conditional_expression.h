#ifndef NODE_CONDITIONAL_EXPRESSION_H
#define NODE_CONDITIONAL_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "node.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

enum class ConditionalType {
  BOOLEAN, NOT, RELATIONAL
};

class ConditionalExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ConditionalType GetConditionalType() = 0;
  [[nodiscard]] virtual bool operator==(const ConditionalExpressionNode &other) const = 0;
  virtual String Accept(DesignExtractor *de, String proc_name, bool is_uses) = 0;
};

}

#endif //NODE_CONDITIONAL_EXPRESSION_H
