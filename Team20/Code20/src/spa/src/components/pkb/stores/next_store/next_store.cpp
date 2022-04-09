#include "next_store.h"
#include "../../pkb_relationship.h"
#include "../parent_store/parent_store.h"
#include <set>

const std::string NextStore::SMALLEST_STMT_NO = "0";
const std::string NextStore::LARGEST_STMT_NO = "501";

NextStore::NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                     std::shared_ptr<ParentStore> parent_store) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)),
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

bool NextStore::IsNextStarPairValid(IDENT_PAIR const &pair) {
  if (std::find(all_star_pairs.begin(), all_star_pairs.end(), pair) != all_star_pairs.end()) {
    return true;
  } else {
    std::unordered_set<std::string> next_star = GetNextStarOf(STMT, pair.first);
    return next_star.find(pair.second) != next_star.end();
  }
}

IDENT_SET NextStore::GetBeforeOf(StmtType type, IDENT const &stmt) {
  return GetUpperSetOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetNextOf(StmtType type, IDENT const &stmt) {
  return GetLowerSetOf(NEXT, type, stmt);
}

IDENT_SET NextStore::GetBeforeStarOf(StmtType type, IDENT const &stmt) {
  std::unordered_set<std::string> res;
  if (star_type_pair_map.find(STMT) != type_pair_map.end()) {
    if (star_type_pair_map.at(STMT).find(type) != type_pair_map.at(type).end()) {
      return GetHelper(STMT, type, 1, stmt, true);
    }
  }
  GetUpperStarOfHelper(stmt, res);
  return res;
}

IDENT_SET NextStore::GetNextStarOf(StmtType type, IDENT const &stmt) {
  std::unordered_set<std::string> res;
  if (star_type_pair_map.find(STMT) != type_pair_map.end()) {
    if (star_type_pair_map.at(STMT).find(type) != type_pair_map.at(type).end()) {
      return GetHelper(STMT, type, 0, stmt, true);
    }
  }
  GetLowerStarOfHelper(stmt, res);
  return res;
}

IDENT_PAIR_VECTOR NextStore::GetNextPairs() {
  return GetAllPairs();
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
  if(GetStarPairByType(type1, type2).empty()) {
    IDENT_PAIR_VECTOR pairs = GetNextStarPairs();
    return GetStarPairByType(type1, type2);
  }
}

void NextStore::GetUpperStarOfHelper(std::string const &stmt,
                                         std::unordered_set<std::string> &res) {
  std::string first_stmt_no;
  StmtType type1 = m_stmt_type->at(stmt);
  for(auto proc : m_proc_stmt_map) {
    std::string first_stmt = proc.second.front();
    std::string last_stmt = proc.second.back();
    if(stoi(stmt) <= stoi(last_stmt) && stoi(stmt) >= stoi(first_stmt)) {
      first_stmt_no = first_stmt;
      break;
    }
  }
  std::unordered_set<std::string> ansc_set = m_parent_store->GetAllAnceOf(WHILE, stmt);
  if(ansc_set.empty()) {
    for(int n = stoi(first_stmt_no); n <= stoi(stmt) - 1; n++) {
      StmtType type2 = m_stmt_type->at(std::to_string(n));
      StmtStmtStore::AddNext(true, type1, std::to_string(n), type2, stmt);
      res.insert(std::to_string(n));
    }
    std::string parent = m_parent_store->GetParentOf(WHILE, std::to_string(stoi(stmt) + 1));
    if(parent == stmt) {
      StmtStmtStore::AddNext(true, type1, stmt, type1, stmt);
      res.insert(stmt);
    }
    return;
  }
  std::string smallest_stmt = LARGEST_STMT_NO;
  for(auto s : ansc_set) {
    if(stoi(s) < stoi(smallest_stmt)) {
      smallest_stmt = s;
    }
  }
  std::unordered_set<std::string> children_set = m_parent_store->GetChildOf(STMT, smallest_stmt);
  std::string largest_stmt = SMALLEST_STMT_NO;
  for(auto s : children_set) {
    if(stoi(s) > stoi(largest_stmt)) {
      largest_stmt = s;
    }
  }
  for(int j = stoi(smallest_stmt); j <= stoi(largest_stmt); j++) {
    StmtType type2 = m_stmt_type->at(std::to_string(j));
    StmtStmtStore::AddNext(true, type1, std::to_string(j), type2, stmt);
    res.insert(std::to_string(j));
  }
  for(int j = stoi(first_stmt_no); j <= stoi(smallest_stmt) - 1; j++) {
    StmtType type2 = m_stmt_type->at(std::to_string(j));
    StmtStmtStore::AddNext(true, type1, std::to_string(j), type2, stmt);
    res.insert(std::to_string(j));
  }
}

void NextStore::GetLowerStarOfHelper(std::string const &stmt,
                                         std::unordered_set<std::string> &res) {
  std::string last_stmt_no;
  StmtType type1 = m_stmt_type->at(stmt);
  for(auto proc : m_proc_stmt_map) {
    std::string first_stmt = proc.second.front();
    std::string last_stmt = proc.second.back();
    if(stoi(stmt) <= stoi(last_stmt) && stoi(stmt) >= stoi(first_stmt)) {
      last_stmt_no = last_stmt;
      break;
    }
  }
  std::unordered_set<std::string> ansc_set = m_parent_store->GetAllAnceOf(WHILE, stmt);
  if(ansc_set.empty()) {
    for(int n = stoi(stmt) + 1; n <= stoi(last_stmt_no); n++) {
      StmtType type2 = m_stmt_type->at(std::to_string(n));
      StmtStmtStore::AddNext(true, type1, stmt, type2, std::to_string(n));
      res.insert(std::to_string(n));
    }
    std::string parent = m_parent_store->GetParentOf(WHILE, std::to_string(stoi(stmt) + 1));
    if(parent == stmt) {
      StmtStmtStore::AddNext(true, type1, stmt, type1, stmt);
      res.insert(stmt);
    }
    return;
  }
  std::string smallest_stmt = LARGEST_STMT_NO;
  for(auto s : ansc_set) {
    if(stoi(s) < stoi(smallest_stmt)) {
      smallest_stmt = s;
    }
  }
  std::unordered_set<std::string> children_set = m_parent_store->GetChildOf(STMT, smallest_stmt);
  std::string largest_stmt = SMALLEST_STMT_NO;
  for(auto s : children_set) {
    if(stoi(s) > stoi(largest_stmt)) {
      largest_stmt = s;
    }
  }
  for(int j = stoi(smallest_stmt); j <= stoi(largest_stmt); j++) {
    StmtType type2 = m_stmt_type->at(std::to_string(j));
    StmtStmtStore::AddNext(true, type1, stmt, type2, std::to_string(j));
    res.insert(std::to_string(j));
  }
  for(int j = stoi(largest_stmt) + 1; j <= stoi(last_stmt_no); j++) {
    StmtType type2 = m_stmt_type->at(std::to_string(j));
    StmtStmtStore::AddNext(true, type1, stmt, type2, std::to_string(j));
    res.insert(std::to_string(j));
  }
}

IDENT_PAIR_VECTOR NextStore::GetNextStarPairs() {
  std::unordered_set<std::string> res;
  for(auto proc : m_proc_stmt_map) {
    int last_stmt = stoi(proc.second.back());
    int first_stmt = stoi(proc.second.front());
    for(int i = first_stmt; i <= last_stmt; i++) {
      GetLowerStarOfHelper(std::to_string(i), res);
    }
  }
  return GetAllStarPairs();
}

void NextStore::ClearNextStarCache() {
  all_star_pairs = IDENT_PAIR_VECTOR();
  star_type_pair_map = NESTED_RELATIONSHIP_MAP();
}