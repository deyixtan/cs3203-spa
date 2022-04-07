#ifndef CFG_CFG_H
#define CFG_CFG_H

#include <string>
#include <unordered_map>
#include <memory>
#include "components/source_subsystem/source_declarations.h"

namespace source {

class Cfg {
 private:
  // <proc_name, cfg_head>
  StringToCfgNodePtrMap m_cfg_map;

 public:
  explicit Cfg(StringToCfgNodePtrMap cfg_map);
  [[nodiscard]] StringToCfgNodePtrMap GetCfgMap();
};

}

#endif //CFG_CFG_H
