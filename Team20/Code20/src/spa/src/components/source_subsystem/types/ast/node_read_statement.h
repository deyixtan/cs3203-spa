#ifndef NODE_READ_STATEMENT_H
#define NODE_READ_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class ReadStatementNode : public StatementNode {
 private:
  VariableNodePtr m_identifier;

 public:
  ReadStatementNode(String stmt_no, VariableNodePtr identifier);
  [[nodiscard]] VariableNodePtr GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
};

}

#endif //NODE_READ_STATEMENT_H
