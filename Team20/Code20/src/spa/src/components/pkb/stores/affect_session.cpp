#include "affect_session.h"
#include "../../source_subsystem/types/cfg/cfg.h"

AffectSession::AffectSession(std::shared_ptr<Cfg> program_cfg, std::string const &a1, std::string const &a2) {
  Populate(program_cfg, a1, a2);
}

void AffectSession::Populate(std::shared_ptr<Cfg> program_cfg, std::string const &a1, std::string const &a2) {
  if (!IsValidArguments(a1, a2)) {
    // TODO: replace with custom exception
    throw std::runtime_error("INCORRECT ARGS");
  }

  // TODO: add Affects CFG extraction logic
}

bool AffectSession::IsValidArguments(std::string const &a1, std::string const &a2) {
  // TODO: add validation logic
  return true;
}

