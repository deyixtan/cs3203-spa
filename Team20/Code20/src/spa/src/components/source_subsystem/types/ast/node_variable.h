#ifndef NODE_VARIABLE_H
#define NODE_VARIABLE_H

#include "components/source_subsystem/source_declarations.h"
#include "node_expression.h"

namespace source {

class VariableNode : public ExpressionNode {
 private:
  String m_identifier;
  String m_stmt;

 public:
  VariableNode();
  explicit VariableNode(String identifier, String stmt);
  [[nodiscard]] String GetIdentifier();
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] String GetPatternFormat() override;
  String Accept(DesignExtractor *de, String proc_name, bool is_uses);
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

}

#endif //NODE_VARIABLE_H
