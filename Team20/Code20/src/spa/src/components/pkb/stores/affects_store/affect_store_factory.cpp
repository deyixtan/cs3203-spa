#include "affect_store_factory.h"
#include "affects_store.h"
#include "../../pkb_relationship.h"

AffectsStoreFactory::AffectsStoreFactory(IDENT_SET_VECTOR_PTR stmt_vector,
                           IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                           ModifiesStorePtr modify_store,
                           UsesStorePtr uses_store) :
    Store(move(stmt_vector), move(stmt_type)),
    m_modify_store(std::move(modify_store)),
    m_uses_store(std::move(uses_store)) {}

std::shared_ptr<source::Cfg> AffectsStoreFactory::GetProgramCfg() {
  return m_program_cfg;
}

ModifiesStorePtr AffectsStoreFactory::GetModifyStore() {
  return m_modify_store;
}

UsesStorePtr AffectsStoreFactory::GetUsesStore() {
  return m_uses_store;
}

bool AffectsStoreFactory::DoesAffectsStoreExist() {
  return m_affects_store != nullptr;
}

AffectsStorePtr AffectsStoreFactory::GetAffectsStore() {
  return m_affects_store;
}

void AffectsStoreFactory::AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg) {
  m_program_cfg = std::move(program_cfg);
}

void AffectsStoreFactory::ComputeAffectsStore() {
  ComputeAffectsStore(true);
}

void AffectsStoreFactory::ComputeAffectsStore(bool is_affect_star_involved) {
  m_affects_store = std::make_shared<AffectsStore>
      (m_stmt_vector, m_stmt_type,
       std::make_shared<AffectsStoreFactory>(*this), is_affect_star_involved);
}

void AffectsStoreFactory::ClearAffectsStore() {
  m_affects_store = nullptr;
}
