#include "affect_store.h"
#include "affect_session.h"

AffectStore::AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                         std::shared_ptr<ModifiesStore> modify_store,
                         std::shared_ptr<UsesStore> usage_store,
                         std::shared_ptr<FollowsStore> follows_store) :
    Store(move(stmt_vector), move(stmt_type)),
    m_modify_store(std::move(modify_store)),
    m_usage_store(std::move(usage_store)),
    m_follows_store(std::move(follows_store)) {}

void AffectStore::AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg) {
  m_program_cfg = std::move(program_cfg);
}

bool AffectStore::DoesAffectSessionExist() {
  return m_affect_session != nullptr;
}

std::shared_ptr<AffectSession> AffectStore::GetAffectSession() {
  return m_affect_session;
}

void AffectStore::ComputeAffectSession() {
  m_affect_session = std::make_shared<AffectSession>(std::make_shared<AffectStore>(*this));
}

void AffectStore::ClearAffectSession() {
  m_affect_session = nullptr;
}

std::shared_ptr<source::Cfg> AffectStore::GetProgramCfg() {
  return m_program_cfg;
}

std::shared_ptr<ModifiesStore> AffectStore::GetModifyStore() {
  return m_modify_store;
}

std::shared_ptr<UsesStore> AffectStore::GetUsageStore() {
  return m_usage_store;
}

std::shared_ptr<FollowsStore> AffectStore::GetFollowsStore() {
  return m_follows_store;
}
