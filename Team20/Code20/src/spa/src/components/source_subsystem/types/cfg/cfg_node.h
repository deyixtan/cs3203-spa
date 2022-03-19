#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_NODE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_NODE_H_

#include <string>
#include <vector>

class CfgNode {
 private:
  std::vector<std::string> m_stmt_no_list;
  std::vector<std::shared_ptr<CfgNode>> m_next_list;

 public:
  CfgNode();
  std::vector<std::string> GetStatementList();
  std::vector<std::shared_ptr<CfgNode>> GetDescendants();
  void AddStatement(std::string stmt_no);
  void AddNext(std::shared_ptr<CfgNode> cfg_node);
};




#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_NODE_H_
