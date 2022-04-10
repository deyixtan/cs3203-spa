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
  std::string first_proc_stmt_no = GetFirstStmtOfProc(stmt);
  std::unordered_set<std::string> ancestor_set = m_parent_store->GetAllAnceOf(WHILE, stmt);
  if(ancestor_set.empty()) { // Not in while loop
    std::unordered_set<std::string> if_ancestor = m_parent_store->GetAllAnceOf(IF, stmt);
    if (!if_ancestor.empty()) {
      InsertUpperResultForIf(first_proc_stmt_no, stmt, stmt, res, if_ancestor.size());
    } else {
      InsertPairResultUpper(stoi(first_proc_stmt_no), stoi(stmt) - 1, res, stmt);
    }
    if(type1 == WHILE) {
      std::string end_while_stmt = GetEndStmtOfWhileLoop(stmt);
      InsertPairResultLower(stoi(stmt), stoi(end_while_stmt), res, stmt);
    }
    return;
  }
  // In a while loop
  std::string start_while_stmt = GetStartStmtOfWhileLoop(ancestor_set);
  std::string end_while_stmt = GetEndStmtOfWhileLoop(start_while_stmt);
  InsertPairResultLower(stoi(start_while_stmt), stoi(end_while_stmt), res, stmt);
  // Check for if container
  std::unordered_set<std::string> if_ancestor = m_parent_store->GetAllAnceOf(IF, start_while_stmt);
  if (!if_ancestor.empty()) {
    InsertUpperResultForIf(first_proc_stmt_no, stmt, start_while_stmt, res, if_ancestor.size());
  } else {
    InsertPairResultUpper(stoi(first_proc_stmt_no), stoi(start_while_stmt) - 1, res, stmt);
  }
}

void NextStore::GetLowerStarOfHelper(std::string const &stmt,
                                         std::unordered_set<std::string> &res) {
  StmtType type1 = m_stmt_type->at(stmt);
  std::string last_proc_stmt_no = GetLastStmtOfProc(stmt);
  std::unordered_set<std::string> ancestor_set = m_parent_store->GetAllAnceOf(WHILE, stmt);
  if(ancestor_set.empty()) { // Not in a while loop
    std::unordered_set<std::string> if_ancestor = m_parent_store->GetAllAnceOf(IF, stmt);
    if (!if_ancestor.empty()) {
      StmtType type = m_stmt_type->at(stmt);
      if (type == IF) {
        std::string end_else_stmt = GetEndElseStmt(stmt);
        InsertPairResultLower(stoi(stmt) + 1, stoi(end_else_stmt), res, stmt);
      }
      InsertLowerResultForIf(last_proc_stmt_no, stmt, stmt, LARGEST_STMT_NO, res, if_ancestor.size());
    } else {
      InsertPairResultLower(stoi(stmt) + 1, stoi(last_proc_stmt_no), res, stmt);
    }
    if(type1 == WHILE) {
      StmtStmtStore::AddNext(true, type1, stmt, type1, stmt);
      res.insert(stmt);
    }
    return;
  }
  // In a while loop
  std::string start_while_stmt = GetStartStmtOfWhileLoop(ancestor_set);
  std::string end_while_stmt = GetEndStmtOfWhileLoop(start_while_stmt);
  InsertPairResultLower(stoi(start_while_stmt), stoi(end_while_stmt), res, stmt);
  // Check for if container
  std::unordered_set<std::string> if_ancestor = m_parent_store->GetAllAnceOf(IF, start_while_stmt);
  if (!if_ancestor.empty()) {
    InsertLowerResultForIf(last_proc_stmt_no, stmt, start_while_stmt, LARGEST_STMT_NO + "0", res, if_ancestor.size());
  } else {
    InsertPairResultLower(stoi(end_while_stmt) + 1, stoi(last_proc_stmt_no), res, stmt);
  }
}

void NextStore::InsertUpperResultForIf(std::string &first_proc_stmt_no, std::string const &stmt, std::string const &prev_if_stmt, std::unordered_set<std::string> &res, int nesting_level) {
  if (nesting_level == 0) {
    InsertPairResultLower(stoi(first_proc_stmt_no), stoi(stmt) - 1, res, stmt);
  } else {
    std::string if_stmt = m_parent_store->GetParentOf(IF, prev_if_stmt);
    std::string first_else_stmt = GetFirstElseStmt(if_stmt);
    std::string end_else_stmt = GetEndElseStmt(if_stmt);
    if (stoi(prev_if_stmt) < stoi(first_else_stmt)) { // in the if block
      if (prev_if_stmt == stmt) {
        InsertPairResultUpper(stoi(if_stmt), stoi(stmt) - 1, res, stmt);
      } else {
        InsertPairResultUpper(stoi(if_stmt), stoi(prev_if_stmt) - 1, res, stmt);
      }
    } else { // in the else block
      InsertPairResultLower(stoi(if_stmt), stoi(if_stmt), res, stmt);
      if (prev_if_stmt == stmt) {
        InsertPairResultUpper(stoi(if_stmt), stoi(stmt) - 1, res, stmt);
      } else {
        InsertPairResultUpper(stoi(first_else_stmt), stoi(prev_if_stmt) - 1, res, stmt);
      }
    }
    InsertUpperResultForIf(first_proc_stmt_no, stmt, if_stmt, res, nesting_level - 1);
  }
}

void NextStore::InsertLowerResultForIf(std::string &last_proc_stmt_no,
                                       std::string const &stmt,
                                       std::string const &prev_if_stmt,
                                       std::string const &prev_end_else_stmt,
                                       std::unordered_set<std::string> &res,
                                       int nesting_level) {
  if (nesting_level == 0) {
    InsertPairResultLower(stoi(prev_end_else_stmt) + 1, stoi(last_proc_stmt_no), res, stmt);
  } else {
    std::string if_stmt = m_parent_store->GetParentOf(IF, prev_if_stmt);
    std::string first_else_stmt = GetFirstElseStmt(if_stmt);
    std::string end_else_stmt = GetEndElseStmt(if_stmt);
    if (stoi(prev_if_stmt) < stoi(first_else_stmt)) { // in the if block
      if (prev_end_else_stmt == LARGEST_STMT_NO) {
        InsertPairResultLower(stoi(stmt) + 1, stoi(first_else_stmt) - 1, res, stmt);
      } else {
        InsertPairResultLower(stoi(prev_end_else_stmt) + 1, stoi(first_else_stmt) - 1, res, stmt);
      }
    } else { // in the else block
      if (prev_end_else_stmt == LARGEST_STMT_NO) {
        InsertPairResultLower(stoi(stmt) + 1, stoi(end_else_stmt), res, stmt);
      } else {
        InsertPairResultLower(stoi(prev_end_else_stmt) + 1, stoi(end_else_stmt), res, stmt);
      }
    }
    InsertLowerResultForIf(last_proc_stmt_no, stmt, if_stmt, end_else_stmt, res, nesting_level - 1);
  }
}

std::string NextStore::GetEndElseStmt(std::string const &if_stmt) {
  std::unordered_set<std::string> if_descendant = m_parent_store->GetAllDescOf(STMT, if_stmt);
  std::string end_else_stmt = SMALLEST_STMT_NO;
  for(auto stmt : if_descendant) {
    if(stoi(stmt) > stoi(end_else_stmt)) {
      end_else_stmt = stmt;
    }
  }
  assert(end_else_stmt != SMALLEST_STMT_NO);
  return end_else_stmt;
}

std::string NextStore::GetFirstElseStmt(std::string const &if_stmt) {
  std::unordered_set<std::string> next_of_if = GetNextOf(STMT, if_stmt);
  std::string first_else_stmt = SMALLEST_STMT_NO;
  for(auto stmt : next_of_if) {
    if(stoi(stmt) > stoi(first_else_stmt)) {
      first_else_stmt = stmt;
    }
  }
  assert(first_else_stmt != SMALLEST_STMT_NO);
  return first_else_stmt;
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

std::string NextStore::GetFirstStmtOfProc(const std::string &stmt) {
  std::string first_stmt_no;
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

std::string NextStore::GetLastStmtOfProc(const std::string &stmt) {
  std::string last_stmt_no;
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

std::string NextStore::GetStartStmtOfWhileLoop(std::unordered_set<std::string> &ansc_set) {
  std::string start_stmt = LARGEST_STMT_NO;
  for(auto stmt : ansc_set) {
    if(stoi(stmt) < stoi(start_stmt)) {
      start_stmt = stmt;
    }
  }
  return start_stmt;
}

std::string NextStore::GetEndStmtOfWhileLoop(const std::string &start_of_while) {
  std::string end_stmt = SMALLEST_STMT_NO;
  std::unordered_set<std::string> children_set = m_parent_store->GetAllDescOf(STMT, start_of_while);
  for(auto stmt : children_set) {
    if(stoi(stmt) > stoi(end_stmt)) {
      end_stmt = stmt;
    }
  }
  return end_stmt;
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