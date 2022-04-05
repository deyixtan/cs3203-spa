#ifndef STATEMENT_H
#define STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "node.h"
#include "../../../pkb/stores/store.h"

namespace source {

class StatementNode : public Node {
 protected:
  String m_stmt_no;

 public:
  explicit StatementNode(String stmt_no);
  [[nodiscard]] String GetStatementNumber();
  [[nodiscard]] virtual StmtType GetStatementType() = 0;
  virtual void Accept(DesignExtractor *de, String proc_name) = 0;
  virtual CfgNodePtr Accept(CfgBuilder *cb, CfgNodePtr cfg_node) = 0;
  virtual bool operator==(const StatementNode &other) const = 0;
};

}

#endif //STATEMENT_H
