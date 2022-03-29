#include "affect_store.h"
#include "affect_session.h"

AffectStore::AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
std::shared_ptr<ModifiesStore> modify_store,
    std::shared_ptr<UsesStore> usage_store) :
  Store(move(stmt_vector)), m_modify_store(modify_store), m_usage_store(usage_store) {}

void AffectStore::AddProgramCfg(std::shared_ptr<Cfg> program_cfg) {
  m_program_cfg = program_cfg;
}

std::shared_ptr<AffectSession> AffectStore::ComputeAffect() {
  std::shared_ptr<AffectSession>
      affect_session = std::make_shared<AffectSession>(std::make_shared<AffectStore>(*this));
  return affect_session;
}

std::shared_ptr<Cfg> AffectStore::GetProgramCfg() {
  return m_program_cfg;
}

std::shared_ptr<ModifiesStore> AffectStore::GetModifyStore() {
  return m_modify_store;
}

std::shared_ptr<UsesStore> AffectStore::GetUsageStore() {
  return m_usage_store;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectStore::GetAllAffectsStmtHelper(
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_affects_pairs,
    StmtType type) {
  std::vector<StmtType> supported_types = {STMT, ASSIGN};
  return Store::GetAllStmt(type, supported_types, all_affects_pairs, false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectStore::GetAllAffectsStmtHelper(
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_affects_pairs,
    StmtType type1,
    StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, ASSIGN};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllAffectsStmtHelper(all_affects_pairs, type2), true);
}
