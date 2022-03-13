#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGWHILENODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGWHILENODE_H_

#include "cfg_group_node.h"

namespace source {

class CfgWhileNode {
 private:
  CfgNode m_node;
  std::shared_ptr<CfgGroupNode> m_body;
  std::shared_ptr<CfgGroupNode> m_next;

 public:
  CfgWhileNode(CfgNode node, std::shared_ptr<CfgGroupNode> body, std::shared_ptr<CfgGroupNode> next);
  [[nodiscard]] CfgNode GetNode();
  [[nodiscard]] std::shared_ptr<CfgGroupNode> GetBody();
  [[nodiscard]] std::shared_ptr<CfgGroupNode> GetNext();
};

};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGWHILENODE_H_
