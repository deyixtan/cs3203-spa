#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_NODE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_NODE_H_

#include <string>
#include <vector>
#include "../../../pkb/stores/store.h"

struct Statement {
  StmtType type;
  std::string stmt_no;
};

class CfgNode {
 private:
  std::vector<Statement> m_stmt_no_list;
  std::vector<std::shared_ptr<CfgNode>> m_next_list;

 public:
  CfgNode();
  std::vector<Statement> GetStatementList();
  std::vector<std::shared_ptr<CfgNode>> GetDescendants();
  void AddStatement(StmtType type, std::string stmt_no);
  void AddNext(std::shared_ptr<CfgNode> cfg_node);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_NODE_H_
