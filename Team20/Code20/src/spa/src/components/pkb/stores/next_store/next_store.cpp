#include "next_store.h"
#include <set>

NextStore::NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                     std::shared_ptr<ParentStore> parent_store) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)),
    m_parent_store(parent_store),
    m_proc_stmt_map({}) {}

void NextStore::AddNext(std::string const &before, std::string const &next) {
  AddUpperLower(NEXT, before, next);
}

void NextStore::AddStmtProc(std::string const &proc, std::string const &stmt) {
  if(m_proc_stmt_map.find(proc) == m_proc_stmt_map.end()) {
    m_proc_stmt_map.insert({proc, {stmt}});
  } else {
    m_proc_stmt_map.at(proc).insert(stmt);
  }
}

std::shared_ptr<ParentStore> NextStore::GetParentStore() {
  return m_parent_store;
}

std::unordered_set<std::string> NextStore::GetStmtProc() {
  return m_proc_stmt_map.at("affectsFiveHundred");
}

bool NextStore::IsNextPairValid(std::pair<std::string, std::string> const &pair) {
  return IsValid(pair);
}

bool NextStore::IsNextStarValid(std::pair<std::string, std::string> const &pair) {
  return IsNextStarPairValid(pair);
}

std::unordered_set<std::string> NextStore::GetBeforeOf(StmtType type, std::string const &stmt) {
  return GetUpperSetOf(NEXT, type, stmt);
}

std::unordered_set<std::string> NextStore::GetNextOf(StmtType type, std::string const &stmt) {
  return GetLowerSetOf(NEXT, type, stmt);
}

std::unordered_set<std::string> NextStore::GetBeforeStarOf(StmtType type, std::string const &stmt) {
  return GetBeforeStarOf(type, stmt);
}

std::unordered_set<std::string> NextStore::GetNextStarOf(StmtType type, std::string const &stmt) {
  return GetLowerStarOf(NEXT, type, stmt);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextPairs() {
  return GetAllPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextStarPairs() {
  return GetAllNextStarPairs();
}

std::unordered_set<std::string> NextStore::GetNextStarSameStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_pairs = GetAllNextStarStmt(type, type);
  std::unordered_set<std::string> same_synonym_set;
  for (auto pair : all_pairs) {
    if (pair.first == pair.second) {
      same_synonym_set.insert(pair.first);
    }
  }
  return same_synonym_set;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetAllNextStmt(StmtType type1,
                                                                                             StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return GetPairByType(type1, type2);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetAllNextStarStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type, supported_types, GetNextStarPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetAllNextStarStmt(StmtType type1,
                                                                                                 StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllNextStarStmt(type2), true);
}
