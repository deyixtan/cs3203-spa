#include "pkb.h"

PKB::PKB()
    : m_stmt_vector(std::make_shared<std::vector<std::unordered_set<std::string>>>(COUNT)),
      m_name_to_stmt(std::make_shared<std::vector<std::unordered_map<std::string, std::unordered_set<std::string>>>>(
          COUNT)), m_stmt_to_name(std::make_shared<std::vector<std::unordered_map<std::string, std::string>>>(
        COUNT)) {
  InitRelationshipStores();
}

void PKB::InitRelationshipStores() {
  m_follow_store = std::make_shared<FollowStore>(m_stmt_vector);
  m_modify_store = std::make_shared<ModifyStore>(m_stmt_vector);
  m_parent_store = std::make_shared<ParentStore>(m_stmt_vector);
  m_usage_store = std::make_shared<UsageStore>(m_stmt_vector);
  m_pattern_store = std::make_shared<PatternStore>(m_stmt_vector);
  m_call_store = std::make_shared<CallStore>(m_stmt_vector);
}

void PKB::AddStmt(std::string const &stmt, StmtType type) {
  m_stmt_vector->at(type).insert(stmt);
}

void PKB::AddNameToStmt(StmtType type, std::string const &name, std::string const &stmt) {
  std::unordered_map<std::string, std::unordered_set<std::string>> ref_map = m_name_to_stmt->at(type);
  if (ref_map.find(stmt) != ref_map.end()) {
    m_name_to_stmt->at(type).at(stmt).insert(name);
  } else{
    m_name_to_stmt->at(type).insert({stmt, {name}});
  }
}

void PKB::AddStmtToName(StmtType type, std::string const &stmt, std::string const &name) {
  std::unordered_map<std::string, std::string> ref_map = m_stmt_to_name->at(type);
  if (ref_map.find(stmt) != ref_map.end()) {
    m_stmt_to_name->at(type).at(stmt) = name;
  } else{
    m_stmt_to_name->at(type).insert({stmt, name});
  }
}

void PKB::AddProgramCfg(std::shared_ptr<Cfg> program_cfg) {
  m_program_cfg = move(program_cfg);
}

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  return m_stmt_vector->at(type);
}

std::unordered_set<std::string> PKB::GetStmtByName(StmtType type, std::string name) {
  return m_name_to_stmt->at(type).at(name);
}

std::string PKB::GetNameByStmt(StmtType type, std::string stmt) {
  return m_stmt_to_name->at(type).at(stmt);
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

std::shared_ptr<CallStore> PKB::GetCallStore() {
  return m_call_store;
}
