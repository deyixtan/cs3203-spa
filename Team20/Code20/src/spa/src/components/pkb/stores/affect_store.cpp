#include "affect_store.h"
#include "affect_session.h"
#include "modify_store.h"
#include "usage_store.h"
#include "../../source_subsystem/types/cfg/cfg.h"

AffectStore::AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<ModifyStore> modify_store,
                         std::shared_ptr<UsageStore> usage_store) :
    Store(move(stmt_vector)), m_modify_store(modify_store), m_usage_store(usage_store) {}

void AffectStore::AddProgramCfg(std::shared_ptr<Cfg> program_cfg) {
  m_program_cfg = program_cfg;
}

std::shared_ptr<AffectSession> AffectStore::ComputeAffect() {
  std::shared_ptr<AffectSession>
      affect_session = std::make_shared<AffectSession>(m_program_cfg, m_modify_store, m_usage_store);
  return affect_session;
}
