#include "next_store.h"

NextStore::NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}


void NextStore::AddNext(std::string const &before, std::string const &next) {
  AddUpperLower(NEXT, before, next);
}

void NextStore::WipeStar() {
  WipeNextStar();
}

bool NextStore::IsNextValid(std::pair<std::string, std::string> const &pair) {
  return IsValid(pair);
}

bool NextStore::IsNextStarValid(std::pair<std::string, std::string> const &pair) {
  return IsStarValid(pair);
}

std::unordered_set<std::string> NextStore::GetBeforeOf(std::string const &stmt) {
  return GetUpperSetOf(NEXT, stmt);
}

std::unordered_set<std::string> NextStore::GetNextOf(StmtType type, std::string const &stmt) {
  return GetLowerSetOf(NEXT, type, stmt);
}

std::unordered_set<std::string> NextStore::GetBeforeStarOf(std::string const &stmt) {
  return GetUpperStarOf(NEXT, stmt);
}

std::unordered_set<std::string> NextStore::GetNextStarOf(std::string const &stmt) {
  return GetLowerStarOf(NEXT, stmt);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextPairs() {
  return GetAllPairs();;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextStarPairs() {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> res;
  for (auto pair : next_rs_map) {
    std::string stmt = pair.first;
    std::unordered_set<std::string> next_star_stmt = GetLowerStarOf(NEXT, stmt);
    for (auto next_star : next_star_stmt){
      res.insert({stmt, next_star});
    }
  }
  return res;
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

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetAllNextStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type, supported_types, GetNextPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetAllNextStmt(StmtType type1,
                                                                                             StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllNextStmt(type2), true);
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
