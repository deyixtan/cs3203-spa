#ifndef NODE_PRINT_STATEMENT_H
#define NODE_PRINT_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class PrintStatementNode : public StatementNode {
 private:
  VariableNodePtr m_identifier;

 public:
  PrintStatementNode(String stmt_no, VariableNodePtr identifier);
  [[nodiscard]] VariableNodePtr GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
};

}

#endif //NODE_PRINT_STATEMENT_H
