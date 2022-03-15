#include "pkb.h"

PKB::PKB() : m_stmt_vector(std::make_shared<std::vector<std::unordered_set<std::string>>>(COUNT)) {
  InitRelationshipStores();
}

void PKB::InitRelationshipStores() {
  m_follow_store = std::make_shared<FollowStore>(m_stmt_vector);
  m_modify_store = std::make_shared<ModifyStore>(m_stmt_vector);
  m_parent_store = std::make_shared<ParentStore>(m_stmt_vector);
  m_usage_store = std::make_shared<UsageStore>(m_stmt_vector);
  m_pattern_store = std::make_shared<PatternStore>(m_stmt_vector);
}

void PKB::AddStmt(std::string const &stmt, StmtType type) {
  m_stmt_vector->at(type).insert(stmt);
}

void PKB::AddProgramCfg(std::shared_ptr<source::CfgProgramNode> program_cfg) {
  m_program_cfg = move(program_cfg);
}

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  return m_stmt_vector->at(type);
}

std::shared_ptr<FollowStore> PKB::GetFollowStore() {
  return m_follow_store;
}

std::shared_ptr<ModifyStore> PKB::GetModifyStore() {
  return m_modify_store;
}

std::shared_ptr<ParentStore> PKB::GetParentStore() {
  return m_parent_store;
}

std::shared_ptr<UsageStore> PKB::GetUsageStore() {
  return m_usage_store;
}

std::shared_ptr<PatternStore> PKB::GetPatternStore() {
  return m_pattern_store;
}
