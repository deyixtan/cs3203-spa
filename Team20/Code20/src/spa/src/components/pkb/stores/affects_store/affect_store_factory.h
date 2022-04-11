#ifndef AFFECTS_STORE_FACTORY_H
#define AFFECTS_STORE_FACTORY_H

#include "components/pkb/pkb_declarations.h"
#include "components/pkb/stores/store.h"
#include "components/source_subsystem/source_declarations.h"

namespace pkb {

class AffectsStoreFactory : public Store {
 private:
  ModifiesStorePtr m_modify_store;
  UsesStorePtr m_uses_store;
  AffectsStorePtr m_affects_store;
  std::shared_ptr<source::Cfg> m_program_cfg;

 public:
  explicit AffectsStoreFactory(IDENT_SET_VECTOR_PTR stmt_vector,
                               IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                               ModifiesStorePtr modify_store,
                               UsesStorePtr uses_store);
  [[nodiscard]] std::shared_ptr<source::Cfg> GetProgramCfg();
  [[nodiscard]] ModifiesStorePtr GetModifyStore();
  [[nodiscard]] UsesStorePtr GetUsesStore();
  [[nodiscard]] bool DoesAffectsStoreExist();
  [[nodiscard]] AffectsStorePtr GetAffectsStore();
  void AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg);
  void ComputeAffectsStore();
  void ComputeAffectsStore(bool is_affects_star_involved);
  void ClearAffectsStore();
};

}

#endif //AFFECTS_STORE_FACTORY_H
