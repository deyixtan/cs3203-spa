#include "stmt_stmt_store.h"

StmtStmtStore::StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector)
    : Store(move(stmt_vector)) {}

void StmtStmtStore::AddUpperLower(StoreType type, std::string const &upper, std::string const &lower) {
  if (type == FOLLOWS) {
    AddFollows(false, upper, lower);
  } else if (type == PARENT) {
    AddParent(false, upper, lower, std::vector<std::string>());
  } else if (type == CALLS) {
    AddCalls(false, upper, lower);
  }
}

void StmtStmtStore::AddUpperLowerStar(StoreType type, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited) {
  if (type == FOLLOWS) {
    AddFollows(true, upper, lower);
  } else if (type == PARENT) {
    AddParent(true, upper, lower, visited);
  } else if (type == CALLS) {
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
    upper_set.insert(upper);
    lower_set.insert(lower);
    all_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
    return;
  }

  follows_rs_map.at(upper).following_star.insert(lower);
  follows_rs_map.at(lower).follower_star.insert(upper);
  upper_star_set.insert(upper);
  lower_star_set.insert(lower);
  all_star_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
}

void StmtStmtStore::AddParent(bool is_star, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited) {
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
    upper_set.insert(upper);
    lower_set.insert(lower);
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
      upper_star_set.insert(ance);
      lower_star_set.insert(lower);
    }
  }
}

void StmtStmtStore::AddCalls(bool is_star, std::string const &upper, std::string const &lower) {
  if (calls_rs_map.find(upper) == calls_rs_map.end()) {
    calls_rs_map.insert({upper, {std::unordered_set<std::string>(), std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                            std::unordered_set<std::string>()}});
  }

  if (calls_rs_map.find(lower) == calls_rs_map.end()) {
    calls_rs_map.insert({lower, {std::unordered_set<std::string>(), std::unordered_set<std::string>(), std::unordered_set<std::string>(),
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

  if (calls_rs_map.find(lower) != calls_rs_map.end()){
    CallsNode node = calls_rs_map.at(lower);
    for (auto callee_star : node.callees_star_set) {
      all_star_pairs.emplace(std::pair<std::string, std::string>(upper, callee_star));
      calls_rs_map.at(callee_star).callers_star_set.insert(upper);
      calls_rs_map.at(upper).callees_star_set.insert(callee_star);
    }
  }

  if (calls_rs_map.find(upper) != calls_rs_map.end()){
    CallsNode node = calls_rs_map.at(upper);
    for (auto caller_star : node.callers_star_set) {
      all_star_pairs.emplace(std::pair<std::string, std::string>(caller_star, lower));
      calls_rs_map.at(caller_star).callees_star_set.insert(lower);
      calls_rs_map.at(lower).callers_star_set.insert(caller_star);
    }
  }
}

bool StmtStmtStore::IsUpper(std::string const &stmt) {
  return upper_set.find(stmt) != upper_set.end();
}

bool StmtStmtStore::IsLower(std::string const &stmt) {
  return lower_set.find(stmt) != lower_set.end();
}

bool StmtStmtStore::IsUpperStar(std::string const &stmt) {
  return upper_star_set.find(stmt) != upper_star_set.end();
}

bool StmtStmtStore::IsLowerStar(std::string const &stmt) {
  return lower_star_set.find(stmt) != lower_star_set.end();
}

bool StmtStmtStore::IsExists(std::pair<std::string, std::string> const &pair) {
  return all_pairs.find(pair) != all_pairs.end();
}

bool StmtStmtStore::IsStarExists(std::pair<std::string, std::string> const &pair) {
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

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetAllPairs() {
  return all_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtStmtStore::GetAllStarPairs() {
  return all_star_pairs;
}