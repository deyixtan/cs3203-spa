#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGIFNODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGIFNODE_H_

#include "cfg_group_node.h"

namespace source {

class CfgIfNode {
 private:
  CfgNode m_node;
  std::shared_ptr<CfgGroupNode> m_next;
  std::shared_ptr<CfgGroupNode> m_alt;

 public:
  CfgIfNode(CfgNode node, std::shared_ptr<CfgGroupNode> next, std::shared_ptr<CfgGroupNode> alt);
  [[nodiscard]] CfgNode GetNode();
  [[nodiscard]] std::shared_ptr<CfgGroupNode> GetNext();
  [[nodiscard]] std::shared_ptr<CfgGroupNode> GetAlt();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGIFNODE_H_
