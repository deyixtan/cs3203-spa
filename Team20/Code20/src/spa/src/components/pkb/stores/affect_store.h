#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECTS_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECTS_STORE_H_

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "store.h"

class AffectSession;
class ModifiesStore;
class UsesStore;
class Cfg;

class AffectStore : public Store {
 private:
  std::shared_ptr<Cfg> m_program_cfg;
  std::shared_ptr<ModifiesStore> m_modify_store;
  std::shared_ptr<UsesStore> m_usage_store;

 public:
  explicit AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                       std::shared_ptr<ModifiesStore> modify_store,
                       std::shared_ptr<UsesStore> usage_store);
  void AddProgramCfg(std::shared_ptr<Cfg> program_cfg);
  std::shared_ptr<AffectSession> ComputeAffect();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECTS_STORE_H_
