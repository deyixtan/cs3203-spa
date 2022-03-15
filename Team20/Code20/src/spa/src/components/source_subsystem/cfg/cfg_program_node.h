#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_PROGRAM_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_PROGRAM_NODE_H_

#include <string>
#include <unordered_map>

#include "cfg_procedure_node.h"
#include "cfg_group_node.h"

namespace source {

class CfgProgramNode {
 private:
  std::unordered_map<std::string, std::shared_ptr<CfgProcedureNode>> m_procedure_map;

 public:
  CfgProgramNode(std::unordered_map<std::string, std::shared_ptr<CfgProcedureNode>> procedure_map);
  std::unordered_map<std::string, std::shared_ptr<CfgProcedureNode>> GetProcedureCfg();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_CFG_CFG_PROGRAM_NODE_H_
