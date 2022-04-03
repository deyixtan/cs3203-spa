#include "stmt_stmt_store.h"

StmtStmtStore::StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type)
    : Store(move(stmt_vector), move(stmt_type)) {}

void StmtStmtStore::AddUpperLower(StoreType store_type,
                                  std::string const &upper,
                                  std::string const &lower) {
  StmtType type1 = PROC;
  StmtType type2 = PROC;

  if (store_type == CALLS) {
    AddCalls(false, upper, lower);
  } else {
    type1 = m_stmt_type->at(upper);
    type2 = m_stmt_type->at(lower);
    if (store_type == FOLLOWS) {
      AddFollows(false, type1, upper, type2, lower);
    } else if (store_type == PARENT) {
      AddParent(false, upper, lower, std::vector<std::string>());
    } else if (store_type == NEXT) {
      AddNext(false, upper, lower);
    }
  }

  ExhaustiveAddStmt(type1, upper, type2, lower, false);
}

void StmtStmtStore::AddUpperLowerStar(StoreType store_type,
                                      std::string const &upper,
                                      std::string const &lower,
                                      std::vector<std::string> const &visited) {

  StmtType type1, type2;

  if (store_type == CALLS) {
    type1 = PROC;
    type2 = PROC;
  } else if (store_type == PARENT) {
    type2 = m_stmt_type->at(lower);
  } else {
    type1 = m_stmt_type->at(lower);
    type2 = m_stmt_type->at(upper);
  }

  if (store_type == FOLLOWS) {
    AddFollows(true, type2, upper, type1, lower);
  } else if (store_type == PARENT) {
    AddParent(true, upper, lower, visited);
  } else if (store_type == CALLS) {
    AddCalls(true, upper, lower);
  } else if (store_type == NEXT) {
    AddNext(true, upper, lower);
  }
}

void StmtStmtStore::AddFollows(bool is_star, StmtType type1, std::string const &upper, StmtType type2, std::string const &lower) {
  if (!is_star) {
    all_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
  } else {
    all_star_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
  }

  ExhaustiveAddStmt(type1, upper, type2, lower, true);
}

void StmtStmtStore::AddParent(bool is_star,
                              std::string const &upper,
                              std::string const &lower,
                              std::vector<std::string> const &visited) {

  if (!is_star) {
    all_pairs.insert(std::make_pair(upper, lower));
    return;
  }

  for (std::string const &ance : visited) {
    if (ance == lower) {
      continue;
    }

    if (ance != "") {
      all_star_pairs.insert(std::make_pair(ance, lower));
      ExhaustiveAddStmt(m_stmt_type->at(ance), ance, m_stmt_type->at(lower), lower, true);
    }
  }
}

void StmtStmtStore::AddNext(bool is_star, std::string const &upper, std::string const &lower) {
  if (next_rs_map.find(upper) == next_rs_map.end()) {
    next_rs_map.insert({upper, {std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                std::unordered_set<std::string>(),
                                std::unordered_set<std::string>()}});
  }

  if (next_rs_map.find(lower) == next_rs_map.end()) {
    next_rs_map.insert({lower, {std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                std::unordered_set<std::string>(),
                                std::unordered_set<std::string>()}});
  }
  all_pairs.insert(std::make_pair(upper, lower));
  next_rs_map.at(lower).before.insert(upper);
  next_rs_map.at(upper).next.insert(lower);
}

void StmtStmtStore::AddCalls(bool is_star, std::string const &upper, std::string const &lower) {
  if (calls_rs_map.find(upper) == calls_rs_map.end()) {
    calls_rs_map.insert({upper, {std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                 std::unordered_set<std::string>(),
                                 std::unordered_set<std::string>()}});
  }

  if (calls_rs_map.find(lower) == calls_rs_map.end()) {
    calls_rs_map.insert({lower, {std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                 std::unordered_set<std::string>(),
                                 std::unordered_set<std::string>()}});
  }

  if (!is_star) {
    calls_rs_map.at(upper).callees_set.insert(lower);
    calls_rs_map.at(lower).callers_set.insert(upper);
    all_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
    return;
  }

  // Calls star
  calls_rs_map.at(upper).callees_star_set.insert(lower);
  calls_rs_map.at(lower).callers_star_set.insert(upper);
  all_star_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
  ExhaustiveAddStmt(PROC, upper, PROC, lower, true);


  if (calls_rs_map.find(lower) != calls_rs_map.end()) {
    CallsNode node = calls_rs_map.at(lower);
    for (auto callee_star : node.callees_star_set) {
      all_star_pairs.emplace(std::pair<std::string, std::string>(upper, callee_star));
      calls_rs_map.at(callee_star).callers_star_set.insert(upper);
      calls_rs_map.at(upper).callees_star_set.insert(callee_star);
      ExhaustiveAddStmt(PROC, upper, PROC, callee_star, true);
    }
  }

  if (calls_rs_map.find(upper) != calls_rs_map.end()) {
    CallsNode node = calls_rs_map.at(upper);
    for (auto caller_star : node.callers_star_set) {
      all_star_pairs.emplace(std::pair<std::string, std::string>(caller_star, lower));
      calls_rs_map.at(caller_star).callees_star_set.insert(lower);
      calls_rs_map.at(lower).callers_star_set.insert(caller_star);
      ExhaustiveAddStmt(PROC, caller_star, PROC, lower, true);
    }
  }
}

void StmtStmtStore::ExhaustiveAddStmt(StmtType type1, std::string upper, StmtType type2, std::string lower, bool is_star) {
  std::unordered_map<StmtType, std::unordered_map<StmtType, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> *pair_map;

  if (!is_star) {
    pair_map = &type_pair_map;
  } else {
    pair_map = &star_type_pair_map;
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> set = {};
  set.insert(std::pair<std::string, std::string>(upper, lower));

  if (pair_map->find(type1) != pair_map->end()) {
    if (pair_map->at(type1).find(type2) != pair_map->at(type1).end()) {
      pair_map->at(type1).at(type2).emplace(std::pair<std::string, std::string>(upper, lower));
    } else {
      pair_map->at(type1).insert({type2, set});
    }
  } else {
    pair_map->insert({type1, {}});
    pair_map->at(type1).insert({type2, set});
  }

  if (pair_map->find(STMT) != pair_map->end()) {
    if (pair_map->at(STMT).find(type2) != pair_map->at(STMT).end()) {
      pair_map->at(STMT).at(type2).emplace(std::pair<std::string, std::string>(upper, lower));
    } else {
      pair_map->at(STMT).insert({type2, set});
    }
  } else {
    pair_map->insert({STMT, {}});
    pair_map->at(STMT).insert({type2, set});
  }

  if (pair_map->find(type1) != pair_map->end()) {
    if (pair_map->at(type1).find(STMT) != pair_map->at(type1).end()) {
      pair_map->at(type1).at(STMT).emplace(std::pair<std::string, std::string>(upper, lower));
    } else {
      pair_map->at(type1).insert({STMT, set});
    }
  } else {
    pair_map->insert({type1, {}});
    pair_map->at(type1).insert({STMT, set});
  }

  if (pair_map->find(STMT) != pair_map->end()) {
    if (pair_map->at(STMT).find(STMT) != pair_map->at(STMT).end()) {
      pair_map->at(STMT).at(STMT).emplace(std::pair<std::string, std::string>(upper, lower));
    } else {
      pair_map->at(STMT).insert({STMT, set});
    }
  } else {
    pair_map->insert({STMT, {}});
    pair_map->at(STMT).insert({STMT, set});
  }
}

void StmtStmtStore::WipeNextStar() {
  for (auto pair : next_rs_map) {
    pair.second.next_star_set = {};
    pair.second.before_star_set = {};
  }
}

bool StmtStmtStore::IsValid(std::pair<std::string, std::string> const &pair) {
  return all_pairs.find(pair) != all_pairs.end();
}

bool StmtStmtStore::IsStarValid(std::pair<std::string, std::string> const &pair) {
  return all_star_pairs.find(pair) != all_star_pairs.end();
}

std::string StmtStmtStore::GetUpperOf(StoreType store_type, StmtType stmt_type, std::string const &stmt) {
  if (type_pair_map.find(stmt_type) != type_pair_map.end()) {
    if (type_pair_map.at(stmt_type).find(STMT) != type_pair_map.at(stmt_type).end()) {
      for (auto &pair : type_pair_map.at(stmt_type).at(STMT)) {
        if (pair.second == stmt) {
          return pair.first;
        }
      }
    }
  }
  return "0";
}

std::string StmtStmtStore::GetLowerOf(StmtType stmt_type, std::string const &stmt) {
  if (type_pair_map.find(STMT) != type_pair_map.end()) {
    if (type_pair_map.at(STMT).find(stmt_type) != type_pair_map.at(STMT).end()) {
      for (auto &pair : type_pair_map.at(stmt_type).at(STMT)) {
        if (pair.first == stmt) {
          return pair.second;
        }
      }
    }
  }
  return "0";
}

std::unordered_set<std::string> StmtStmtStore::GetUpperSetOf(StoreType store_type, std::string const &stmt) {
  std::unordered_set<std::string> result;

  if (store_type == NEXT) {
    if (next_rs_map.find(stmt) != next_rs_map.end()) {
      return next_rs_map.at(stmt).before;
    }
  } else if (store_type == CALLS) {
    if (type_pair_map.find(PROC) != type_pair_map.end()) {
      if (type_pair_map.at(PROC).find(PROC) != type_pair_map.at(PROC).end()) {
        for (auto &pair : type_pair_map.at(PROC).at(PROC)) {
          if (pair.second == stmt) {
            result.insert(pair.first);
          }
        }
        return result;
      }
    }
  } else {
    return {};
  }
}

std::unordered_set<std::string> StmtStmtStore::GetLowerSetOf(StoreType store_type, StmtType stmt_type, std::string const &stmt) {
  std::unordered_set<std::string> result;

  if (store_type == NEXT) {
    if (next_rs_map.find(stmt) != next_rs_map.end()) {
      return next_rs_map.at(stmt).next;
    }
  }

  if (type_pair_map.find(STMT) != type_pair_map.end()) {
    if (type_pair_map.at(STMT).find(stmt_type) != type_pair_map.at(STMT).end()) {
      for (auto &pair : type_pair_map.at(STMT).at(stmt_type)) {
        if (pair.first == stmt) {
          result.insert(pair.second);
        }
      }
      return result;
    }
  }

  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetUpperStarOf(StoreType store_type, StmtType stmt_type, std::string const &stmt) {
  std::unordered_set<std::string> result;

  if (store_type == NEXT) {
    if (next_rs_map.find(stmt) != next_rs_map.end() && !next_rs_map.at(stmt).before_star_set.empty()) {
      return next_rs_map.at(stmt).before_star_set;
    }
    std::unordered_set<std::string> res;
    std::unordered_set<std::string> visited;
    GetUpperStarOfHelper(stmt, res, visited);
    return res;
  }

  if (star_type_pair_map.find(stmt_type) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(stmt_type).find(STMT) != star_type_pair_map.at(stmt_type).end()) {
      for (auto &pair : star_type_pair_map.at(stmt_type).at(STMT)) {
        if (pair.second == stmt) {
          result.insert(pair.first);
        }
      }
      return result;
    }
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerStarOf(StoreType store_type, StmtType stmt_type, std::string const &stmt) {
  std::unordered_set<std::string> result;

  if (store_type == NEXT) {
    if (next_rs_map.find(stmt) != next_rs_map.end() && !next_rs_map.at(stmt).next_star_set.empty()) {
      return next_rs_map.at(stmt).next_star_set;
    }
    std::unordered_set<std::string> res;
    std::unordered_set<std::string> visited;
    GetLowerStarOfHelper(stmt, res, visited);
    return res;
  }

  if (star_type_pair_map.find(stmt_type) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(stmt_type).find(STMT) != star_type_pair_map.at(stmt_type).end()) {
      for (auto &pair : star_type_pair_map.at(stmt_type).at(STMT)) {
        if (pair.first == stmt) {
          result.insert(pair.second);
        }
      }
      return result;
    }
  }

  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetPairByType(StmtType type1,
                                                                                                StmtType type2) {
  if (type_pair_map.find(type1) != type_pair_map.end()) {
    if (type_pair_map.at(type1).find(type2) != type_pair_map.at(type1).end()) {
      return type_pair_map.at(type1).at(type2);
    }
  }
  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetStarPairByType(StmtType type1,
                                                                                                    StmtType type2) {
  if (star_type_pair_map.find(type1) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(type1).find(type2) != star_type_pair_map.at(type1).end()) {
      return star_type_pair_map.at(type1).at(type2);
    }
  }
  return {};
}

void StmtStmtStore::GetUpperStarOfHelper(std::string const &stmt,
                                    std::unordered_set<std::string> &res,
                                    std::unordered_set<std::string> &visited) {
  if (next_rs_map.find(stmt) == next_rs_map.end()) {
    return;
  }
  if(visited.find(stmt) != visited.end()) {
    bool is_loop = false;
    for (auto next : visited) {
      if(next == stmt) {
        is_loop = true;
      }
      next_rs_map[next].before_star_set.insert(stmt);
      if(is_loop) {
        next_rs_map[next].before_star_set.insert(next);
      }
      res.insert(stmt);
    }
    return;
  }
  for (auto next : visited) {
    next_rs_map[next].before_star_set.insert(stmt);
    res.insert(stmt);
  }
  visited.insert(stmt);
  std::unordered_set<std::string> &before_set = next_rs_map[stmt].before;
  for (auto before: before_set) {
    std::unordered_set<std::string> new_visited;
    for (auto stmt : visited) {
      new_visited.insert(stmt);
    }
    GetLowerStarOfHelper(before, res, new_visited);
  }
}

void StmtStmtStore::GetLowerStarOfHelper(std::string const &stmt,
                                  std::unordered_set<std::string> &res,
                                  std::unordered_set<std::string> &visited) {
  if (next_rs_map.find(stmt) == next_rs_map.end()) {
    return;
  }
  if(visited.find(stmt) != visited.end()) {
    bool is_loop = false;
    for (auto before : visited) {
      if(before == stmt) {
        is_loop = true;
      }
      next_rs_map[before].next_star_set.insert(stmt);
      if (is_loop) {
        next_rs_map[before].next_star_set.insert(before);
      }
      res.insert(stmt);
    }
    return;
  }
  for (auto before : visited) {
    next_rs_map[before].next_star_set.insert(stmt);
    res.insert(stmt);
  }
  visited.insert(stmt);
  std::unordered_set<std::string> &next_set = next_rs_map[stmt].next;
  for (auto next: next_set) {
    std::unordered_set<std::string> new_visited;
    for (auto stmt : visited) {
      new_visited.insert(stmt);
    }
    GetLowerStarOfHelper(next, res, new_visited);
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetAllPairs() {
  return all_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetAllStarPairs() {
  return all_star_pairs;
}