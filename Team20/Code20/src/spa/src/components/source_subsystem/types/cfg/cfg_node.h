#ifndef CFG_NODE_H
#define CFG_NODE_H

#include "components/source_subsystem/source_declarations.h"
#include "components/pkb/pkb.h"

namespace source {

// encapsulates all the information
// needed in a cfg statement
struct CfgNodeStatement {
  StmtType type;
  String stmt_no;
};

class CfgNode {
 private:
  CfgNodeStatementStream m_stmt_list;
  CfgNodeStream m_next_list;

 public:
  CfgNode();
  [[nodiscard]] CfgNodeStatementStream GetStatementList();
  [[nodiscard]] CfgNodeStream GetNextList();
  void AddStatement(StmtType type, String &stmt_no);
  void AddNext(CfgNodePtr &cfg_node);
};

}

#endif //CFG_NODE_H
