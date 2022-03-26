#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_

#include <memory>
#include <string>

class Cfg;

class AffectSession {
 public:
  explicit AffectSession(std::shared_ptr<Cfg> program_cfg, std::string const &a1, std::string const &a2);
  void Populate(std::shared_ptr<Cfg> program_cfg, std::string const &a1, std::string const &a2);
  bool IsValidArguments(std::string const &a1, std::string const &a2);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_
