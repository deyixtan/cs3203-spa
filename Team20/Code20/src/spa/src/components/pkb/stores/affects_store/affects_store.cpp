#include "affects_store.h"
#include "affects_session.h"

AffectsStore::AffectsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                         std::shared_ptr<ModifiesStore> modify_store,
                         std::shared_ptr<UsesStore> usage_store,
                         std::shared_ptr<FollowsStore> follows_store) :
    Store(move(stmt_vector), move(stmt_type)),
    m_modify_store(std::move(modify_store)),
    m_usage_store(std::move(usage_store)),
    m_follows_store(std::move(follows_store)) {}

void AffectsStore::AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg) {
  m_program_cfg = std::move(program_cfg);
}

bool AffectsStore::DoesAffectsSessionExist() {
  return m_affects_session != nullptr;
}

std::shared_ptr<AffectsSession> AffectsStore::GetAffectsSession() {
  return m_affects_session;
}

void AffectsStore::ComputeAffectsSession() {
  ComputeAffectsSession(true);
}

void AffectsStore::ComputeAffectsSession(bool is_affect_star_involved) {
  m_affects_session = std::make_shared<AffectsSession>(m_stmt_vector, m_stmt_type, std::make_shared<AffectsStore>(*this), is_affect_star_involved);
}

void AffectsStore::ClearAffectsSession() {
  m_affects_session = nullptr;
}

std::shared_ptr<source::Cfg> AffectsStore::GetProgramCfg() {
  return m_program_cfg;
}

std::shared_ptr<ModifiesStore> AffectsStore::GetModifyStore() {
  return m_modify_store;
}

std::shared_ptr<UsesStore> AffectsStore::GetUsageStore() {
  return m_usage_store;
}

std::shared_ptr<FollowsStore> AffectsStore::GetFollowsStore() {
  return m_follows_store;
}
