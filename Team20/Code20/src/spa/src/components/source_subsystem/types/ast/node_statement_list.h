#ifndef NODE_STATEMENT_LIST_H
#define NODE_STATEMENT_LIST_H

#include "components/source_subsystem/source_declarations.h"
#include "node.h"

namespace source {

class StatementListNode : public Node {
 private:
  StatementNodeStream m_statements;

 public:
  StatementListNode();
  explicit StatementListNode(StatementNodeStream statements);
  [[nodiscard]] StatementNodeStream GetStatements();
  void Accept(DesignExtractor *de, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementListNode &other) const;
};

}

#endif //NODE_STATEMENT_LIST_H
