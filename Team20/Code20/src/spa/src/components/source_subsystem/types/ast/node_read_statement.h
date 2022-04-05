#ifndef READ_STATEMENT_H
#define READ_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "node_statement.h"
#include "node_variable.h"

namespace source {

class ReadStatementNode : public StatementNode {
 private:
  VariableNodePtr m_identifier;

 public:
  ReadStatementNode(String stmt_no, VariableNodePtr identifier);
  [[nodiscard]] VariableNodePtr GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] String GetPatternFormat() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
};

}

#endif //READ_STATEMENT_H
