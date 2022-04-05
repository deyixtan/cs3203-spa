#ifndef NODE_CONDITIONAL_EXPRESSION_H
#define NODE_CONDITIONAL_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

enum class ConditionalType {
  BOOLEAN, NOT, RELATIONAL
};

class ConditionalExpressionNode : public Node {
 public:
  virtual String Accept(DesignExtractor *design_extractor, String proc_name, bool is_uses) = 0;
  [[nodiscard]] virtual bool operator==(const ConditionalExpressionNode &other) const = 0;
};

}

#endif //NODE_CONDITIONAL_EXPRESSION_H
