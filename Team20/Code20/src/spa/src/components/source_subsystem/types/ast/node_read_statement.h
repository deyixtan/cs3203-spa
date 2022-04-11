#ifndef NODE_READ_STATEMENT_H
#define NODE_READ_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class ReadStatementNode : public StatementNode {
 private:
  VariableNodePtr m_variable;

 public:
  ReadStatementNode(String &stmt_no, VariableNodePtr variable);
  [[nodiscard]] VariableNodePtr GetVariable();
  void Accept(DesignExtractorPtr design_extractor) override;
  void Accept(CfgBuilderPtr cfg_builder) override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

}

#endif //NODE_READ_STATEMENT_H
