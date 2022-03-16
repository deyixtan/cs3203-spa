#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGNODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGNODE_H_

#include <vector>

namespace source {

class CfgNode {
 private:
  int m_stmt_no;

 public:
  CfgNode(int stmt_no);
  [[nodiscard]] int GetStatementNumber();
};

};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFGNODE_H_
