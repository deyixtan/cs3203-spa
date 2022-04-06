#ifndef NODE_VARIABLE_H
#define NODE_VARIABLE_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_expression.h"

namespace source {

class VariableNode : public ExpressionNode {
 private:
  String m_name;
  String m_stmt_no;

 public:
  explicit VariableNode(String name, String stmt_no);
  [[nodiscard]] String GetName();
  [[nodiscard]] String GetPatternFormat() override;
  void Accept(DesignExtractorPtr design_extractor) override;
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

}

#endif //NODE_VARIABLE_H
