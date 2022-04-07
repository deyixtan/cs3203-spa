#ifndef NODE_EXPRESSION_H
#define NODE_EXPRESSION_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

class ExpressionNode : public Node {
 public:
  [[nodiscard]] virtual String GetPatternFormat() = 0;
  void Accept(DesignExtractorPtr design_extractor) override = 0;
  [[nodiscard]] virtual bool operator==(const ExpressionNode &other) const = 0;
};

}

#endif //NODE_EXPRESSION_H
