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

void NextStore::AddFirstStmtProc(std::string const &proc, std::string const &stmt) {
    m_proc_stmt_map.insert({proc, {stmt}});
}

void NextStore::AddLastStmtProc(std::string const &proc, std::string const &stmt) {
  m_proc_stmt_map.at(proc).push_back(stmt);
}

bool NextStore::IsNextPairValid(IDENT_PAIR const &pair) {
  return IsValid(pair);
}

bool NextStore::IsNextStarValid(IDENT_PAIR const &pair) {
  return IsNextStarPairValid(pair);
}

void NextStore::WipeNextStar() {
  return ClearNextStarCache();
}

IDENT_SET NextStore::GetBeforeOf(StmtType type, IDENT const &stmt) {
  return GetUpperSetOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetNextOf(StmtType type, IDENT const &stmt) {
  return GetLowerSetOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetBeforeStarOf(StmtType type, IDENT const &stmt) {
  std::unordered_set<std::string> res;
  GetUpperStarOfHelper(stmt, res, m_proc_stmt_map);
  return res;
}

IDENT_SET NextStore::GetNextStarOf(StmtType type, IDENT const &stmt) {
  std::unordered_set<std::string> res;
  GetLowerStarOfHelper(stmt, res, m_proc_stmt_map);
  return res;
}

IDENT_PAIR_VECTOR NextStore::GetNextPairs() {
  return GetAllPairs();
}

IDENT_PAIR_VECTOR NextStore::GetNextStarPairs() {
  return GetAllNextStarPairs(m_proc_stmt_map);
}

IDENT_SET NextStore::GetNextStarSameStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  std::vector<std::pair<std::string, std::string>> all_pairs = GetAllNextStarStmt(type, type);
  std::unordered_set<std::string> same_synonym_set;
  for (auto pair : all_pairs) {
    if (pair.first == pair.second) {
      same_synonym_set.insert(pair.first);
    }
  }
  return same_synonym_set;
}

IDENT_PAIR_VECTOR NextStore::GetAllNextStmt(StmtType type1, StmtType type2) {
  return GetPairByType(type1, type2);
}

IDENT_PAIR_VECTOR NextStore::GetAllNextStarStmt(StmtType type1, StmtType type2) {
  return GetNextStarPairs();
}
