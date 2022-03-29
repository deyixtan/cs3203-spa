#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECTS_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECTS_STORE_H_

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "store.h"

class AffectSession;
class ModifyStore;
class UsageStore;
class Cfg;

class AffectStore : public Store {
 private:
  std::shared_ptr<Cfg> m_program_cfg;
  std::shared_ptr<ModifyStore> m_modify_store;
  std::shared_ptr<UsageStore> m_usage_store;

 public:
  explicit AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                       std::shared_ptr<ModifyStore> modify_store,
                       std::shared_ptr<UsageStore> usage_store);
  void AddProgramCfg(std::shared_ptr<Cfg> program_cfg);
  std::shared_ptr<AffectSession> ComputeAffect();
  [[nodiscard]] std::shared_ptr<Cfg> GetProgramCfg();
  [[nodiscard]] std::shared_ptr<ModifyStore> GetModifyStore();
  [[nodiscard]] std::shared_ptr<UsageStore> GetUsageStore();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllAffectsStmtHelper(
      std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_affects_pairs, StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllAffectsStmtHelper(
      std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_affects_pairs, StmtType type1, StmtType type2);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECTS_STORE_H_
