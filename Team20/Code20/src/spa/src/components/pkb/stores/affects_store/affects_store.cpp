#include "affects_store.h"
#include "affects_session.h"

AffectsStore::AffectsStore(IDENT_SET_VECTOR_PTR stmt_vector,
                           IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                           ModifiesStorePtr modify_store,
                           UsesStorePtr uses_store) :
    Store(move(stmt_vector), move(stmt_type)),
    m_modify_store(std::move(modify_store)),
    m_uses_store(std::move(uses_store)) {}

std::shared_ptr<source::Cfg> AffectsStore::GetProgramCfg() {
  return m_program_cfg;
}

ModifiesStorePtr AffectsStore::GetModifyStore() {
  return m_modify_store;
}

UsesStorePtr AffectsStore::GetUsesStore() {
  return m_uses_store;
}

bool AffectsStore::DoesAffectsSessionExist() {
  return m_affects_session != nullptr;
}

std::shared_ptr<AffectsSession> AffectsStore::GetAffectsSession() {
  return m_affects_session;
}

void AffectsStore::AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg) {
  m_program_cfg = std::move(program_cfg);
}

void AffectsStore::ComputeAffectsSession() {
  ComputeAffectsSession(true);
}

void AffectsStore::ComputeAffectsSession(bool is_affect_star_involved) {
  m_affects_session = std::make_shared<AffectsSession>
      (m_stmt_vector, m_stmt_type,
       std::make_shared<AffectsStore>(*this), is_affect_star_involved);
}

void AffectsStore::ClearAffectsSession() {
  m_affects_session = nullptr;
}
