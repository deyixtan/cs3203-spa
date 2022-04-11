#include "cfg.h"

namespace source {

Cfg::Cfg(StringToCfgNodePtrMap cfg_map) : m_cfg_map(std::move(cfg_map)) {}

StringToCfgNodePtrMap Cfg::GetCfgMap() {
  return m_cfg_map;
}

}
