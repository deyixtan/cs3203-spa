#include "pkb.h"
#include "pkb_relationship.h"
#include "components/pkb/stores/affects_store/affects_store.h"
#include "components/pkb/stores/affects_store/affect_store_factory.h"
#include "components/pkb/stores/call_store/call_store.h"
#include "components/pkb/stores/follows_store/follows_store.h"
#include "components/pkb/stores/modifies_store/modifies_store.h"
#include "components/pkb/stores/next_store/next_store.h"
#include "components/pkb/stores/parent_store/parent_store.h"
#include "components/pkb/stores/pattern_store/pattern_store.h"
#include "components/pkb/stores/uses_store/uses_store.h"
#include "components/source_subsystem/types/cfg/cfg.h"

namespace pkb {

PKB::PKB()
    : m_stmt_vector(std::make_shared<std::vector<std::unordered_set<std::string>>>(COUNT)),
      m_stmt_type(std::make_shared<std::unordered_map<std::string, StmtType>>()),
      m_name_vector(std::make_shared<std::vector<std::unordered_set<std::string>>>(COUNT)),
      m_name_to_stmt(std::make_shared<std::vector<std::unordered_map<std::string, std::unordered_set<std::string>>>>(
          COUNT)), m_stmt_to_name(std::make_shared<std::vector<std::unordered_map<std::string, std::string>>>(
        COUNT)) {
  InitRelationshipStores();
}

void PKB::InitRelationshipStores() {
  m_follow_store = std::make_shared<FollowsStore>(m_stmt_vector, m_stmt_type);
  m_modify_store = std::make_shared<ModifiesStore>(m_stmt_vector, m_stmt_type);
  m_parent_store = std::make_shared<ParentStore>(m_stmt_vector, m_stmt_type);
  m_usage_store = std::make_shared<UsesStore>(m_stmt_vector, m_stmt_type);
  m_pattern_store = std::make_shared<PatternStore>(m_stmt_vector, m_stmt_type);
  m_call_store = std::make_shared<CallStore>(m_stmt_vector, m_stmt_type);
  m_next_store = std::make_shared<NextStore>(m_stmt_vector, m_stmt_type, m_parent_store);
  m_affects_store_factory = std::make_shared<AffectsStoreFactory>(
      m_stmt_vector, m_stmt_type, m_modify_store, m_usage_store);
}

void PKB::AddStmt(std::string const &stmt, StmtType type) {
  m_stmt_vector->at(type).insert(stmt);
}

void PKB::AddTypeOfStmt(std::string const &stmt, StmtType type) {
  m_stmt_type->insert({stmt, type});
}

void PKB::AddName(std::string const &name, StmtType type) {
  m_name_vector->at(type).insert(name);
}

void PKB::AddNameToStmt(StmtType type, std::string const &name, std::string const &stmt) {
  std::unordered_map<std::string, std::unordered_set<std::string>> ref_map = m_name_to_stmt->at(type);
  if (ref_map.find(name) != ref_map.end()) {
    m_name_to_stmt->at(type).at(name).insert(stmt);
  } else {
    m_name_to_stmt->at(type).insert({name, {stmt}});
  }
}

void PKB::AddStmtToName(StmtType type, std::string const &stmt, std::string const &name) {
  std::unordered_map<std::string, std::string> ref_map = m_stmt_to_name->at(type);
  if (ref_map.find(stmt) != ref_map.end()) {
    m_stmt_to_name->at(type).at(stmt) = name;
  } else {
    m_stmt_to_name->at(type).insert({stmt, name});
  }
}

void PKB::AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg) {
  m_program_cfg = program_cfg;
  m_affects_store_factory->AddProgramCfg(program_cfg);
}

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  return m_stmt_vector->at(type);
}

StmtType PKB::GetTypeOfStmt(std::string stmt_no) {
  if (m_stmt_type->count(stmt_no) == 0)
    // TODO: add custom exception
    throw std::runtime_error("NO SUCH STMT_NO");
  return m_stmt_type->at(stmt_no);
}

std::unordered_set<std::string> PKB::GetName(StmtType type) {
  return m_name_vector->at(type);
}

std::unordered_set<std::string> PKB::GetStmtByName(StmtType type, std::string name) {
  std::unordered_map<std::string, std::unordered_set<std::string>> ref_map = m_name_to_stmt->at(type);

  if (ref_map.find(name) != ref_map.end()) {
    return ref_map.at(name);
  }
  return {};
}

std::string PKB::GetNameByStmt(StmtType type, std::string stmt) {
  std::unordered_map<std::string, std::string> ref_map = m_stmt_to_name->at(type);

  if (ref_map.find(stmt) != ref_map.end()) {
    return ref_map.at(stmt);
  }
  return "";
}

std::shared_ptr<FollowsStore> PKB::GetFollowsStore() {
  return m_follow_store;
}

std::shared_ptr<ModifiesStore> PKB::GetModifiesStore() {
  return m_modify_store;
}

std::shared_ptr<ParentStore> PKB::GetParentStore() {
  return m_parent_store;
}

std::shared_ptr<UsesStore> PKB::GetUsesStore() {
  return m_usage_store;
}

std::shared_ptr<PatternStore> PKB::GetPatternStore() {
  return m_pattern_store;
}

std::shared_ptr<CallStore> PKB::GetCallStore() {
  return m_call_store;
}

std::shared_ptr<NextStore> PKB::GetNextStore() {
  return m_next_store;
}

AffectsStoreFactoryPtr PKB::GetAffectsStoreFactory() {
  return m_affects_store_factory;
}

std::shared_ptr<source::Cfg> PKB::GetProgCfg() {
  return m_program_cfg;
}

}
