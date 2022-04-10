#include "next_store.h"
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
    GetNextStarPairs();
  }
  return GetStarPairByType(type1, type2);
}

void NextStore::GetUpperStarOfHelper(std::string const &stmt,
                                         std::unordered_set<std::string> &res) {
  StmtType type1 = m_stmt_type->at(stmt);
  std::string first_stmt_no = GetFirstStmtOfProc(stmt, first_stmt_no);
  std::unordered_set<std::string> ansc_set = m_parent_store->GetAllAnceOf(WHILE, stmt);
  if(ansc_set.empty()) {
    InsertPairResultUpper(stoi(first_stmt_no), stoi(stmt) - 1, res, stmt);
    if(type1 == WHILE) {
      StmtStmtStore::AddNext(true, type1, stmt, type1, stmt);
      res.insert(stmt);
    }
    return;
  }
  std::string smallest_stmt = GetStartStmtOfWhileLoop(ansc_set);
  std::string largest_stmt = GetEndStmtOfWhileLoop(smallest_stmt);
  InsertPairResultUpper(stoi(smallest_stmt), stoi(largest_stmt), res, stmt);
  InsertPairResultUpper(stoi(first_stmt_no), stoi(smallest_stmt) - 1, res, stmt);
}

void NextStore::InsertPairResultUpper(int start, int end, std::unordered_set<std::string> &res, std::string const stmt) {
  StmtType type1 = m_stmt_type->at(stmt);
  for (int i = start; i <= end; i++) {
    StmtType type2 = m_stmt_type->at(std::to_string(i));
    StmtStmtStore::AddNext(true, type1, std::to_string(i), type2, stmt);
    res.insert(std::to_string(i));
  }
}

void NextStore::InsertPairResultLower(int start, int end, std::unordered_set<std::string> &res, std::string const stmt) {
  StmtType type1 = m_stmt_type->at(stmt);
  for (int i = start; i <= end; i++) {
    StmtType type2 = m_stmt_type->at(std::to_string(i));
    StmtStmtStore::AddNext(true, type1, stmt, type2, std::to_string(i));
    res.insert(std::to_string(i));
  }
}

std::string &NextStore::GetFirstStmtOfProc(const std::string &stmt, std::string &first_stmt_no) {
  for(auto proc : m_proc_stmt_map) {
    std::string first_stmt = proc.second.front();
    std::string last_stmt = proc.second.back();
    if(stoi(stmt) <= stoi(last_stmt) && stoi(stmt) >= stoi(first_stmt)) {
      first_stmt_no = first_stmt;
      break;
    }
  }
  return first_stmt_no;
}

void NextStore::GetLowerStarOfHelper(std::string const &stmt,
                                         std::unordered_set<std::string> &res) {
  StmtType type1 = m_stmt_type->at(stmt);
  std::string last_stmt_no = GetLastStmtOfProc(stmt, last_stmt_no);
  std::unordered_set<std::string> ansc_set = m_parent_store->GetAllAnceOf(WHILE, stmt);
  std::unordered_set<std::string> if_set = m_parent_store->GetAllAnceOf(IF, stmt);
  if(ansc_set.empty()) {

    // TODO: If stmt is in a if block, do not add lines from else block
    if (!if_set.empty()) {
      std::string start_if_stmt = LARGEST_STMT_NO;
      for(auto s : if_set) {
        if(stoi(s) > stoi(start_if_stmt)) {
          start_if_stmt = s;
        }
      }

      std::string end_if_stmt = SMALLEST_STMT_NO;
      for(auto s : if_set) {
        if(stoi(s) > stoi(end_if_stmt)) {
          end_if_stmt = s;
        }
      }

      for (int i = stoi(start_if_stmt); i < stoi(end_if_stmt); i++) {
        // TODO: check if current and next has the Next relationship
      }
    }

    InsertPairResultLower(stoi(stmt) + 1, stoi(last_stmt_no), res, stmt);
    if(type1 == WHILE) {
      StmtStmtStore::AddNext(true, type1, stmt, type1, stmt);
      res.insert(stmt);
    }
    return;
  }
  std::string smallest_stmt = GetStartStmtOfWhileLoop(ansc_set);
  std::string largest_stmt = GetEndStmtOfWhileLoop(smallest_stmt);
  InsertPairResultLower(stoi(smallest_stmt), stoi(largest_stmt), res, stmt);
  InsertPairResultLower(stoi(largest_stmt) + 1, stoi(last_stmt_no), res, stmt);
}

std::string NextStore::GetStartStmtOfWhileLoop(std::unordered_set<std::string> &ansc_set) const {
  std::string smallest_stmt = LARGEST_STMT_NO;
  for(auto s : ansc_set) {
    if(stoi(s) < stoi(smallest_stmt)) {
      smallest_stmt = s;
    }
  }
  return smallest_stmt;
}

std::string NextStore::GetEndStmtOfWhileLoop(const std::string &start_of_while) {
  std::string largest_stmt = SMALLEST_STMT_NO;
  std::unordered_set<std::string> children_set = m_parent_store->GetChildOf(STMT, start_of_while);
  for(auto s : children_set) {
    if(stoi(s) > stoi(largest_stmt)) {
      largest_stmt = s;
    }
  }
  return largest_stmt;
}

std::string &NextStore::GetLastStmtOfProc(const std::string &stmt, std::string &last_stmt_no) {
  for(auto proc : m_proc_stmt_map) {
    std::string first_stmt = proc.second.front();
    std::string last_stmt = proc.second.back();
    if(stoi(stmt) <= stoi(last_stmt) && stoi(stmt) >= stoi(first_stmt)) {
      last_stmt_no = last_stmt;
      break;
    }
  }
  return last_stmt_no;
}

void NextStore::GetNextStarPairs() {
  std::unordered_set<std::string> res;
  for(auto proc : m_proc_stmt_map) {
    int last_stmt = stoi(proc.second.back());
    int first_stmt = stoi(proc.second.front());
    for(int i = first_stmt; i <= last_stmt; i++) {
      GetLowerStarOfHelper(std::to_string(i), res);
    }
  }
}

void NextStore::ClearNextStarCache() {
  all_star_pairs = IDENT_PAIR_VECTOR();
  star_type_pair_map = NESTED_TUPLE_MAP();
}