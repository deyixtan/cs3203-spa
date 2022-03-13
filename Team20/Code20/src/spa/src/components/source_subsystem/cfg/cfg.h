#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_H_

#include <string>
#include <unordered_map>

#include "cfg_group_node.h"

namespace source {

class ProgramCfg {
 private:
  std::unordered_map<std::string, std::shared_ptr<CfgGroupNode>> m_procedure_map;

 public:
  ProgramCfg(std::unordered_map<std::string, std::shared_ptr<CfgGroupNode>> procedure_map);
  std::unordered_map<std::string, std::shared_ptr<CfgGroupNode>> GetProcedureCfg();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_H_
