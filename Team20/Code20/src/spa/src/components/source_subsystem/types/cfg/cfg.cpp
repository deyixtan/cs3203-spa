#include "cfg.h"

Cfg::Cfg(std::unordered_map<std::string, std::shared_ptr<CfgNode>> heads) {
  m_heads = heads;
}

std::shared_ptr<CfgNode> Cfg::GetCfg(std::string procedure) {
  return m_heads.at(procedure);
}

std::unordered_map<std::string, std::shared_ptr<CfgNode>> Cfg::GetCfgMap() {
  return m_heads;
}
