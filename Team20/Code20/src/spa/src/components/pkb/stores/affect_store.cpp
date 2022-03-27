#include "affect_store.h"
#include "affect_session.h"
#include "../../source_subsystem/types/cfg/cfg.h"

AffectStore::AffectStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    Store(move(stmt_vector)) {}

void AffectStore::AddProgramCfg(std::shared_ptr<Cfg> program_cfg) {
  m_program_cfg = program_cfg;
}

std::shared_ptr<AffectSession> AffectStore::ComputeAffect() {
  std::shared_ptr<AffectSession> affect_session = std::make_shared<AffectSession>(m_program_cfg);
  return affect_session;
}
