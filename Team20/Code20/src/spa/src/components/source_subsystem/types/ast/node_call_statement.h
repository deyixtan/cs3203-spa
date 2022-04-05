#ifndef NODE_CALL_STATEMENT_H
#define NODE_CALL_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "node_statement.h"

namespace source {

class CallStatementNode : public StatementNode {
 private:
  String m_identifier;
  String proc_name;

 public:
  CallStatementNode(String stmt_no, String proc_name, String m_identifier);
  [[nodiscard]] String GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

}

#endif //NODE_CALL_STATEMENT_H
