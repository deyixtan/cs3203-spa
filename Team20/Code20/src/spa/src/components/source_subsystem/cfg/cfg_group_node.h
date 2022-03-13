#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGGROUPNODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGGROUPNODE_H_

#include <vector>

#include "cfg_node.h"

namespace source {

class CfgGroupNode {
 private:
  std::vector<CfgNode> m_nodes;
  std::shared_ptr<CfgGroupNode> m_next;

 public:
  CfgGroupNode();
  CfgGroupNode(std::vector<CfgNode> nodes);
  CfgGroupNode(std::vector<CfgNode> nodes, std::shared_ptr<CfgGroupNode> next);
  [[nodiscard]] std::vector<CfgNode> &GetNodes();
  [[nodiscard]] std::shared_ptr<CfgGroupNode> &GetNext();

};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGGROUPNODE_H_
