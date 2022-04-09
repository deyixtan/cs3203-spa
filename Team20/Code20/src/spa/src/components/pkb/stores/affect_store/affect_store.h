#ifndef AFFECT_STORE_H
#define AFFECT_STORE_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "components/source_subsystem/source_declarations.h"
#include "components/pkb/stores/store.h"

class ModifiesStore;
class UsesStore;
class FollowsStore;
class AffectSession;

class AffectStore : public Store {
 private:
  std::shared_ptr<source::Cfg> m_program_cfg;
  std::shared_ptr<ModifiesStore> m_modify_store;
  std::shared_ptr<UsesStore> m_usage_store;
  std::shared_ptr<FollowsStore> m_follows_store;
  std::shared_ptr<AffectSession> m_affect_session;

 public:
  explicit AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                       std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                       std::shared_ptr<ModifiesStore> modify_store,
                       std::shared_ptr<UsesStore> usage_store,
                       std::shared_ptr<FollowsStore> follow_store);
  void AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg);
  [[nodiscard]] bool DoesAffectSessionExist();
  [[nodiscard]] std::shared_ptr<AffectSession> GetAffectSession();
  void ComputeAffectSession();
  void ComputeAffectSession(bool is_affect_star_involved);
  void ClearAffectSession();
  [[nodiscard]] std::shared_ptr<source::Cfg> GetProgramCfg();
  [[nodiscard]] std::shared_ptr<ModifiesStore> GetModifyStore();
  [[nodiscard]] std::shared_ptr<UsesStore> GetUsageStore();
  [[nodiscard]] std::shared_ptr<FollowsStore> GetFollowsStore();
};

#endif //AFFECT_STORE_H
