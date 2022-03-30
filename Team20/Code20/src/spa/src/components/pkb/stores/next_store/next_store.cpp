#include "next_store.h"

NextStore::NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StmtStmtStore(move(stmt_vector)) {}

void NextStore::AddNextMap(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map) {
  next_map = rs_map;
}

void NextStore::AddBeforeMap(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map) {
  std::unordered_map<std::string, std::unordered_set<std::string>> new_map;
  for (auto pairs : rs_map) {
    std::string before = pairs.first;
    for (auto stmt : pairs.second) {
      if (new_map.find(stmt) == new_map.end()) {
        std::unordered_set<std::string> before_set = std::unordered_set<std::string>();
        before_set.insert(before);
        new_map.insert({stmt, before_set});
      } else {
        std::unordered_set<std::string> vals = new_map[stmt];
        vals.insert(before);
        new_map[stmt] = vals;
      }
    }
  }
  before_map = new_map;
}

void NextStore::ConstructNextPairs() {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> pairs;
  for (auto pair : next_map) {
    std::string stmt = pair.first;
    for (auto next : pair.second) {
      pairs.insert({stmt, next});
    }
  }
  all_next_pairs = pairs;
}

bool NextStore::IsNext(std::string const &stmt) {
  for (auto pairs : all_next_pairs) {
    if (pairs.second == stmt) {
      return true;
    }
  }
  return false;
}

bool NextStore::IsBefore(std::string const &stmt) {
  for (auto pairs : all_next_pairs) {
    if (pairs.first == stmt) {
      return true;
    }
  }
  return false;
}

bool NextStore::IsNextStar(std::string const &stmt) {
  return IsNext(stmt);
}

bool NextStore::IsBeforeStar(const std::string &stmt) {
  return IsBefore(stmt);
}

bool NextStore::IsNextValid(std::pair<std::string, std::string> const &pair) {
  if (all_next_pairs.find(pair) != all_next_pairs.end()) {
    return true;
  } else {
    return false;
  }
}

bool NextStore::IsNextStarValid(std::pair<std::string, std::string> const &pair) {
  std::unordered_set<std::string> res;
  std::unordered_set<std::string> visited;
  FindNextStarOf(pair.first, res, visited);
  if (res.find(pair.second) != res.end()) {
    return true;
  } else {
    return false;
  }
}

std::unordered_set<std::string> NextStore::GetBeforeOf(std::string const &stmt) {
  std::unordered_set<std::string> res;
  for (auto pairs : all_next_pairs) {
    if (pairs.second == stmt) {
      res.insert(pairs.first);
    }
  }
  return res;
}

std::unordered_set<std::string> NextStore::GetNextOf(std::string const &stmt) {
  std::unordered_set<std::string> next_set;
  for (auto pairs : all_next_pairs) {
    if (pairs.first == stmt) {
      next_set.insert(pairs.second);
    }
  }
  return next_set;
}

std::unordered_set<std::string> NextStore::GetBeforeStarOf(std::string const &stmt) {
  std::unordered_set<std::string> res;
  std::unordered_set<std::string> visited;
  FindBeforeStarOf(stmt, res, visited);
  return res;
}

void NextStore::FindBeforeStarOf(std::string const &stmt, std::unordered_set<std::string> &res, std::unordered_set<std::string> &visited) {
  if (before_map.find(stmt) == before_map.end() || visited.find(stmt) != visited.end()) {
    return;
  }
  std::unordered_set<std::string> before_set = before_map[stmt];
  visited.insert(stmt);
  for (auto before : before_set) {
    res.insert(before);
    FindBeforeStarOf(before, res, visited);
  }
}

std::unordered_set<std::string> NextStore::GetNextStarOf(std::string const &stmt) {
  std::unordered_set<std::string> res;
  std::unordered_set<std::string> visited;
  FindNextStarOf(stmt, res, visited);
  return res;
}

void NextStore::FindNextStarOf(std::string const &stmt, std::unordered_set<std::string> &res, std::unordered_set<std::string> &visited) {
  if (next_map.find(stmt) == next_map.end() || visited.find(stmt) != visited.end()) {
    return;
  }
  std::unordered_set<std::string> &next_set = next_map[stmt];
  visited.insert(stmt);
  for (auto next : next_set) {
    res.insert(next);
    FindNextStarOf(next, res, visited);
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextPairs() {
  return all_next_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextStarPairs() {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> res;
  for (auto pair : next_map) {
    std::string stmt = pair.first;
    std::unordered_set<std::string> next_star_stmt = GetNextStarOf(stmt);
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
  return Store::GetAllStmt(type, supported_types, all_next_pairs, false);
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
