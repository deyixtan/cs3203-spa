#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_PROCEDURE_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_PROCEDURE_NODE_H_

#include <string>

#include "cfg_group_node.h"

namespace source {

class CfgProcedureNode {
 private:
  std::vector<std::shared_ptr<CfgGroupNode>> m_nodes;

 public:
  CfgProcedureNode();
  [[ndiscard]] std::shared_ptr<CfgGroupNode> GetLastNode();
  void AddNode(std::shared_ptr<CfgGroupNode> cfg_node);
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_PROCEDURE_NODE_H_
