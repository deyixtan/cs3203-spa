#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_H_

#include <string>
#include <unordered_map>
#include <memory>

namespace source {

class CfgNode;

class Cfg {
 private:
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> m_heads;

 public:
  Cfg(std::unordered_map<std::string, std::shared_ptr<CfgNode>> heads);
  std::shared_ptr<CfgNode> GetCfg(std::string procedure);
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> GetCfgMap();
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CFG_CFG_H_
