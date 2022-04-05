#ifndef NODE_PROCEDURE_H
#define NODE_PROCEDURE_H

#include "components/source_subsystem/source_declarations.h"
#include "node.h"

namespace source {

class ProcedureNode : public Node {
 private:
  String m_identifier;
  StatementListNodePtr m_stmt_list;

 public:
  ProcedureNode(String identifier, StatementListNodePtr stmt_list);
  [[nodiscard]] String GetIdentifier();
  [[nodiscard]] StatementListNodePtr GetStatementList();
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const ProcedureNode &other) const;
  void Accept(DesignExtractor *de);
};

}

#endif //NODE_PROCEDURE_H
