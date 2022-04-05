#ifndef NODE_CONSTANT_H
#define NODE_CONSTANT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_expression.h"

namespace source {

class ConstantNode : public ExpressionNode {
 private:
  String m_value;

 public:
  explicit ConstantNode(String value);
  [[nodiscard]] String GetValue();
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] String GetPatternFormat() override;
  std::string Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

}

#endif //NODE_CONSTANT_H
