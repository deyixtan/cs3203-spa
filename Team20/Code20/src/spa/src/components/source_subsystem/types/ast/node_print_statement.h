#ifndef NODE_PRINT_STATEMENT_H
#define NODE_PRINT_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class PrintStatementNode : public StatementNode {
 private:
  VariableNodePtr m_variable;

 public:
  PrintStatementNode(String &stmt_no, VariableNodePtr variable);
  [[nodiscard]] VariableNodePtr GetVariable();
  void Accept(DesignExtractorPtr design_extractor) override;
  //void Accept(DesignExtractor *design_extractor, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  StmtType GetStatementType();
};

}

#endif //NODE_PRINT_STATEMENT_H
