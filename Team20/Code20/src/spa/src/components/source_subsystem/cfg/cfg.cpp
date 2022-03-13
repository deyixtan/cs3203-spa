#include "cfg.h"

namespace source {

ProgramCfg::ProgramCfg(std::unordered_map<std::string, std::shared_ptr<CfgGroupNode>> procedure_map) :
    m_procedure_map(procedure_map) {}

std::unordered_map<std::string, std::shared_ptr<CfgGroupNode>> ProgramCfg::GetProcedureCfg() {
  return m_procedure_map;
}

}
