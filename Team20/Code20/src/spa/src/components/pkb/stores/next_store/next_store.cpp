#include "next_store.h"
#include <set>

NextStore::NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                     std::shared_ptr<ParentStore> parent_store) :
    StmtStmtStore(move(stmt_vector), move(stmt_type), parent_store),
    m_parent_store(parent_store),
    m_proc_stmt_map({}) {}

void NextStore::AddNext(IDENT const &before, IDENT const &next) {
  AddUpperLower(NEXT, before, next);
}

void NextStore::AddStmtProc(std::string const &proc, std::string const &stmt) {
  if(m_proc_stmt_map.find(proc) == m_proc_stmt_map.end()) {
    m_proc_stmt_map.insert({proc, {stmt}});
  } else {
    m_proc_stmt_map.at(proc).insert(stmt);
  }
}

bool NextStore::IsNextPairValid(IDENT_PAIR const &pair) {
  return IsValid(pair);
}

bool NextStore::IsNextStarValid(IDENT_PAIR const &pair) {
  return IsNextStarPairValid(pair);
}

IDENT_SET NextStore::GetBeforeOf(StmtType type, IDENT const &stmt) {
  return GetUpperSetOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetNextOf(StmtType type, IDENT const &stmt) {
  return GetLowerSetOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetBeforeStarOf(StmtType type, IDENT const &stmt) {
  return GetUpperStarOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetNextStarOf(StmtType type, IDENT const &stmt) {
  return GetLowerStarOf(NEXT, type, stmt);
}

IDENT_PAIR_SET NextStore::GetNextPairs() {
  return GetAllPairs();
}

IDENT_PAIR_SET NextStore::GetNextStarPairs() {
  return GetAllNextStarPairs();
}

IDENT_SET NextStore::GetNextStarSameStmt(StmtType type) {
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

IDENT_PAIR_SET NextStore::GetAllNextStmt(StmtType type1, StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return GetPairByType(type1, type2);
}

IDENT_PAIR_SET NextStore::GetAllNextStarStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type, supported_types, GetNextStarPairs(), false);
}

IDENT_PAIR_SET NextStore::GetAllNextStarStmt(StmtType type1, StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllNextStarStmt(type2), true);
}
