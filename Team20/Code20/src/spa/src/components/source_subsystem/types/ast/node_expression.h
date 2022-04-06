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
  void Accept(DesignExtractorPtr design_extractor) = 0;
  CfgNodePtr Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) = 0;
  [[nodiscard]] virtual bool operator==(const ExpressionNode &other) const = 0;
};

}

#endif //NODE_EXPRESSION_H
