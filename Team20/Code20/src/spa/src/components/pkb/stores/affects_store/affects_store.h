#ifndef AFFECTS_STORE_H
#define AFFECTS_STORE_H

#include "components/pkb/common.h"
#include "components/pkb/stores/store.h"
#include "components/source_subsystem/source_declarations.h"

class ModifiesStore;
class UsesStore;
class AffectsSession;

class AffectsStore : public Store {
 private:
  ModifiesStorePtr m_modify_store;
  UsesStorePtr m_uses_store;
  AffectsSessionPtr m_affects_session;
  std::shared_ptr<source::Cfg> m_program_cfg;

 public:
  explicit AffectsStore(IDENT_SET_VECTOR_PTR stmt_vector,
                        IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                        ModifiesStorePtr modify_store,
                        UsesStorePtr uses_store);
  [[nodiscard]] std::shared_ptr<source::Cfg> GetProgramCfg();
  [[nodiscard]] ModifiesStorePtr GetModifyStore();
  [[nodiscard]] UsesStorePtr GetUsesStore();
  [[nodiscard]] bool DoesAffectsSessionExist();
  [[nodiscard]] AffectsSessionPtr GetAffectsSession();
  void AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg);
  void ComputeAffectsSession();
  void ComputeAffectsSession(bool is_affects_star_involved);
  void ClearAffectsSession();
};

#endif //AFFECTS_STORE_H
