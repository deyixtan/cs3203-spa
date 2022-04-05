#ifndef NODE_EXPRESSION_H
#define NODE_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

enum class ExpressionType {
  CONSTANT, COMBINATION, VARIABLE
};

class ExpressionNode : public Node {
 public:
  [[nodiscard]] virtual String GetPatternFormat() = 0;
  [[nodiscard]] virtual ExpressionType GetExpressionType() = 0;
  [[nodiscard]] virtual bool operator==(const ExpressionNode &other) const = 0;
  virtual String Accept(DesignExtractor *de, String proc_name, bool is_uses) = 0;
};

}

#endif //NODE_EXPRESSION_H
