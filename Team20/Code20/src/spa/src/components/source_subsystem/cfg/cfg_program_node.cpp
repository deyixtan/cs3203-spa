#include "cfg_program_node.h"

namespace source {

CfgProgramNode::CfgProgramNode(std::unordered_map<std::string, std::shared_ptr<CfgProcedureNode>> procedure_map) :
    m_procedure_map(procedure_map) {}

std::unordered_map<std::string, std::shared_ptr<CfgProcedureNode>> CfgProgramNode::GetProcedureCfg() {
  return m_procedure_map;
}

}
