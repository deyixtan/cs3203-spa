#include "stmt_stmt_store.h"

StmtStmtStore::StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type)
    : Store(move(stmt_vector), move(stmt_type)) {}

void StmtStmtStore::AddUpperLower(StoreType store_type,
                                  std::string const &upper,
                                  std::string const &lower) {
  StmtType type1, type2;

  if (store_type == CALLS) {
    type1 = CALL;
    type2 = CALL;
  }

  if (store_type == PARENT) {
    type1 = m_stmt_type->at(upper);
    type2 = m_stmt_type->at(lower);
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> set = {};
  set.insert(std::pair<std::string, std::string>(upper, lower));

  if (type_pair_map.find(type1) != type_pair_map.end()) {
    if (type_pair_map.at(type1).find(type2) != type_pair_map.at(type1).end()) {
      type_pair_map.at(type1).at(type2).emplace(std::pair<std::string, std::string>(upper, lower));
    } else {
      type_pair_map.at(type1).insert({type2, set});
    }
  } else {
    type_pair_map.insert({type1, {}});
    type_pair_map.at(type1).insert({type2, set});
  }

  if (store_type == FOLLOWS) {
    AddFollows(false, upper, lower);
  } else if (store_type == PARENT) {
    AddParent(false, upper, lower, std::vector<std::string>());
  } else if (store_type == CALLS) {
    AddCalls(false, upper, lower);
  }
}

void StmtStmtStore::AddUpperLowerStar(StoreType store_type,
                                      std::string const &upper,
                                      std::string const &lower,
                                      std::vector<std::string> const &visited) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> set = {};

  StmtType type1, type2;

  if (store_type == CALLS) {
    type1 = CALL;
    type2 = CALL;
  }

  if (store_type == PARENT) {
    type2 = m_stmt_type->at(lower);
  }

  if (store_type != PARENT) {
    set.insert(std::pair<std::string, std::string>(upper, lower));

    if (star_type_pair_map.find(type1) != star_type_pair_map.end()) {
      if (star_type_pair_map.at(type1).find(type2) != star_type_pair_map.at(type1).end()) {
        star_type_pair_map.at(type1).at(type2).emplace(std::pair<std::string, std::string>(upper, lower));
      } else {
        star_type_pair_map.at(type1).insert({type2, set});
      }
    } else {
      star_type_pair_map.insert({type1, {}});
      star_type_pair_map.at(type1).insert({type2, set});
    }
  }

  if (store_type == FOLLOWS) {
    AddFollows(true, upper, lower);
  } else if (store_type == PARENT) {
    AddParent(true, upper, lower, visited);
  } else if (store_type == CALLS) {
    AddCalls(true, upper, lower);
  }
}

void StmtStmtStore::AddFollows(bool is_star, std::string const &upper, std::string const &lower) {
  if (follows_rs_map.find(upper) == follows_rs_map.end()) {
    follows_rs_map.insert({upper, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (follows_rs_map.find(lower) == follows_rs_map.end()) {
    follows_rs_map.insert({lower, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (!is_star) {
    follows_rs_map.at(upper).following = lower;
    follows_rs_map.at(lower).follower = upper;
    all_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
    return;
  }

  follows_rs_map.at(upper).following_star.insert(lower);
  follows_rs_map.at(lower).follower_star.insert(upper);
  all_star_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
}

void StmtStmtStore::AddParent(bool is_star,
                              std::string const &upper,
                              std::string const &lower,
                              std::vector<std::string> const &visited) {
  StmtType type1 = m_stmt_type->at(lower);
  if (parent_rs_map.find(upper) == parent_rs_map.end()) {
    parent_rs_map.insert({upper, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                  std::unordered_set<std::string>()}});
  }

  if (parent_rs_map.find(lower) == parent_rs_map.end()) {
    parent_rs_map.insert({lower, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                  std::unordered_set<std::string>()}});
  }

  if (!is_star) {
    all_pairs.insert(std::make_pair(upper, lower));
    parent_rs_map.at(upper).child.insert(lower);
    parent_rs_map.at(lower).parent = upper;
    return;
  }

  for (std::string const &ance : visited) {
    if (parent_rs_map.find(ance) == parent_rs_map.end()) {
      parent_rs_map.insert({ance, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                                   std::unordered_set<std::string>()}});
    }

    if (ance != lower) {
      all_star_pairs.insert(std::make_pair(ance, lower));
      parent_rs_map.at(lower).ance.insert(ance);
      parent_rs_map.at(ance).desc.insert(lower);
    }

    std::unordered_set<std::pair<std::string, std::string>, pair_hash> set = {};
    set.insert(std::pair<std::string, std::string>(ance, lower));
    StmtType ance_type = m_stmt_type->at(ance);

    if (star_type_pair_map.find(type1) != star_type_pair_map.end()) {
      if (star_type_pair_map.at(type1).find(ance_type) != star_type_pair_map.at(type1).end()) {
        star_type_pair_map.at(type1).at(ance_type).emplace(std::pair<std::string, std::string>(upper, lower));
      } else {
        star_type_pair_map.at(type1).insert({ance_type, set});
      }
    } else {
      star_type_pair_map.insert({type1, {}});
      star_type_pair_map.at(type1).insert({ance_type, set});
    }
  }
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

  if (calls_rs_map.find(lower) != calls_rs_map.end()) {
    CallsNode node = calls_rs_map.at(lower);
    for (auto callee_star : node.callees_star_set) {
      all_star_pairs.emplace(std::pair<std::string, std::string>(upper, callee_star));
      calls_rs_map.at(callee_star).callers_star_set.insert(upper);
      calls_rs_map.at(upper).callees_star_set.insert(callee_star);
    }
  }

  if (calls_rs_map.find(upper) != calls_rs_map.end()) {
    CallsNode node = calls_rs_map.at(upper);
    for (auto caller_star : node.callers_star_set) {
      all_star_pairs.emplace(std::pair<std::string, std::string>(caller_star, lower));
      calls_rs_map.at(caller_star).callees_star_set.insert(lower);
      calls_rs_map.at(lower).callers_star_set.insert(caller_star);
    }
  }
}

bool StmtStmtStore::IsValid(std::pair<std::string, std::string> const &pair) {
  return all_pairs.find(pair) != all_pairs.end();
}

bool StmtStmtStore::IsStarValid(std::pair<std::string, std::string> const &pair) {
  return all_star_pairs.find(pair) != all_star_pairs.end();
}

std::string StmtStmtStore::GetUpperOf(StoreType type, std::string const &stmt) {
  if (type == FOLLOWS) {
    if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
      return follows_rs_map.at(stmt).follower;
    }
  } else if (type == PARENT) {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).parent;
    }
  }
  return "0";
}

std::unordered_set<std::string> StmtStmtStore::GetUpperOf(std::string const &stmt) {
  if (calls_rs_map.find(stmt) != calls_rs_map.end()) {
    return calls_rs_map.at(stmt).callers_set;
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerOf(StoreType type, std::string const &stmt) {
  if (type == PARENT) {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).child;
    }
  } else {
    if (calls_rs_map.find(stmt) != calls_rs_map.end()) {
      return calls_rs_map.at(stmt).callees_set;
    }
  }
  return {};
}

std::string StmtStmtStore::GetLowerOf(std::string const &stmt) {
  if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
    return follows_rs_map.at(stmt).following;
  }
  return "0";
}

std::unordered_set<std::string> StmtStmtStore::GetUpperStarOf(StoreType type, std::string const &stmt) {
  if (type == FOLLOWS) {
    if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
      return follows_rs_map.at(stmt).follower_star;
    }
  } else if (type == PARENT) {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).ance;
    }
  } else if (type == CALLS) {
    if (calls_rs_map.find(stmt) != calls_rs_map.end()) {
      return calls_rs_map.at(stmt).callers_star_set;
    }
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerStarOf(StoreType type, std::string const &stmt) {
  if (type == FOLLOWS) {
    if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
      return follows_rs_map.at(stmt).following_star;
    }
  } else if (type == PARENT) {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).desc;
    }
  } else if (type == CALLS) {
    if (calls_rs_map.find(stmt) != calls_rs_map.end()) {
      return calls_rs_map.at(stmt).callees_star_set;
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

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetAllPairs() {
  return all_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetAllStarPairs() {
  return all_star_pairs;
}