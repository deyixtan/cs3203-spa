#include "pkb.h"

PKB::PKB()
    : m_follow_store(FollowStore(m_stmt_vector)),
      m_modify_store(ModifyStore(m_stmt_vector)),
      m_parent_store(ParentStore(m_stmt_vector)),
      m_usage_store(UsageStore(m_stmt_vector)) {
  InitStatementVector();
}

FollowStore &PKB::GetFollowStore() {
  return m_follow_store;
}

ModifyStore &PKB::GetModifyStore() {
  return m_modify_store;
}

ParentStore &PKB::GetParentStore() {
  return m_parent_store;
}

UsageStore &PKB::GetUsageStore() {
  return m_usage_store;
}

void PKB::InitStatementVector() {
  for (int i = 0; i < COUNT; i++) {
    std::unordered_set<std::string> tmp_list;
    m_stmt_vector.push_back(tmp_list);
  }
}

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  return m_stmt_vector.at(type);
}

void PKB::AddStmt(std::string stmt, StmtType type) {
  m_stmt_vector.at(type).insert(stmt);
}

void PKB::AddPattern(std::string stmt, std::string lhs, std::string rhs) {
  m_pattern_map[stmt] = {lhs, rhs};
}

std::unordered_set<std::string> PKB::GetStmtWithPattern(std::string lhs, std::string rhs) {
  std::unordered_set<std::string> result = {};
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());
  //rhs = "(" + rhs + ")";
  //TODO: Tree builder and parser

  for (auto const&[key, val] : m_pattern_map) {
    if (lhs == "_" && rhs == "_") {
      result.insert(key);
    }

    if (lhs == "_" && rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    if (lhs == "_" && rhs != "_" && rhs.find("_") == std::string::npos) {
      if (val.second == rhs) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs == "_") {
      if (lhs == val.first) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (lhs == val.first && val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs != "_" && rhs.find("_") == std::string::npos) {
      if (lhs == val.first && val.second == rhs) {
        result.insert(key);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetStmtWithPatternSynonym(std::string rhs) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());

  for (auto const&[key, val] : m_pattern_map) {
    if (rhs == "_") {
      result.insert({key, val.first});
    }

    // Exact match
    if (rhs != "_" && rhs.find("_") == std::string::npos) {
      if (val.second == rhs) {
        result.insert({key, val.first});
      }
    }

    // Partial match
    if (rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert({key, val.first});
      }
    }
  }

  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllModStmt(StmtType type) {
  if (type != STMT && type != READ && type != ASSIGN && type != IF && type != WHILE) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> mod_stmt_var_list = m_modify_store.GetAllStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : mod_stmt_var_list) {
    for (auto j : m_stmt_vector.at(type)) {
      if (i.first == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllUsesStmt(StmtType type) {
  if (type != STMT && type != WHILE && type != PRINT && type != IF && type != ASSIGN) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> uses_stmt_list = m_usage_store.GetAllStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : uses_stmt_list) {
    for (auto j : m_stmt_vector.at(type)) {
      if (i.first == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStmt(StmtType type) {
  if (type != STMT && type != READ && type != ASSIGN && type != WHILE && type != PRINT && type != IF) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_stmt_list = m_follow_store.GetFollowPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : follow_stmt_list) {
    for (auto j : m_stmt_vector.at(type)) {
      if (i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStmt(StmtType type1,
                                                                                         StmtType type2) {
  if (type1 != STMT && type1 != READ && type1 != ASSIGN && type1 != WHILE && type1 != PRINT && type1 != IF) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_stmt_list = GetAllFollowStmt(type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : follow_stmt_list) {
    for (auto j : m_stmt_vector.at(type1)) {
      if (i.first == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStarStmt(StmtType type) {
  if (type != STMT && type != READ && type != ASSIGN && type != WHILE && type != PRINT && type != IF) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      follow_star_stmt_list = m_follow_store.GetFollowStarPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : follow_star_stmt_list) {
    for (auto j : m_stmt_vector.at(type)) {
      if (i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStarStmt(StmtType type1,
                                                                                             StmtType type2) {
  if (type1 != STMT && type1 != READ && type1 != ASSIGN && type1 != WHILE && type1 != PRINT && type1 != IF) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      follow_star_stmt_list = GetAllFollowStarStmt(type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : follow_star_stmt_list) {
    for (auto j : m_stmt_vector.at(type1)) {
      if (i.first == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStmt(StmtType type) {
  if (type != STMT && type != ASSIGN && type != PRINT && type != READ && type != IF && type != WHILE) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      parent_child_list = m_parent_store.GetParentChildPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : parent_child_list) {
    for (auto j : m_stmt_vector.at(type)) {
      if (i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStmt(StmtType type1,
                                                                                         StmtType type2) {
  if (type1 != STMT && type1 != IF && type1 != WHILE) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      parent_child_list = GetAllParentStmt(type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : parent_child_list) {
    for (auto j : m_stmt_vector.at(type1)) {
      if (i.first == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStarStmt(StmtType type) {
  if (type != STMT && type != ASSIGN && type != PRINT && type != READ && type != IF && type != WHILE) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> ance_desc_list = m_parent_store.GetAnceDescPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : ance_desc_list) {
    for (auto j : m_stmt_vector.at(type)) {
      if (i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStarStmt(StmtType type1,
                                                                                             StmtType type2) {
  if (type1 != STMT && type1 != IF && type1 != WHILE) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> ance_desc_list = GetAllParentStarStmt(type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : ance_desc_list) {
    for (auto j : m_stmt_vector.at(type1)) {
      if (i.first == j) {
        result.insert(i);
      }
    }
  }
  return result;
}
