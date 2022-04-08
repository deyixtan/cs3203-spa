#include "stmt_stmt_store.h"
#include "../pkb_relationship.h"

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
      AddNext(false, type1, upper, type2, lower);
    }
  }

  ExhaustiveAddAllStmt(type1, upper, type2, lower, false);
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
    AddNext(true, type2, upper, type1, lower);
  }
}

void StmtStmtStore::AddFollows(bool is_star,
                               StmtType type1,
                               std::string const &upper,
                               StmtType type2,
                               std::string const &lower) {
  if (!is_star) {
    all_pairs.push_back({upper, lower});
  } else {
    all_star_pairs.push_back({upper, lower});
    ExhaustiveAddAllStmt(type1, upper, type2, lower, true);
  }
}

void StmtStmtStore::AddParent(bool is_star,
                              std::string const &upper,
                              std::string const &lower,
                              std::vector<std::string> const &visited) {

  if (!is_star) {
    all_pairs.push_back({upper, lower});
    return;
  }

  for (std::string const &ance : visited) {
    if (ance == lower) {
      continue;
    }

    if (ance != "") {
      all_star_pairs.push_back({ance, lower});
      ExhaustiveAddAllStmt(m_stmt_type->at(ance), ance, m_stmt_type->at(lower), lower, true);
    }
  }
}

void StmtStmtStore::AddNext(bool is_star,
                            StmtType type1,
                            std::string const &upper,
                            StmtType type2,
                            std::string const &lower) {
  if (!is_star) {
    all_pairs.push_back(std::pair<std::string, std::string>(upper, lower));
  } else {
    all_star_pairs.push_back({upper, lower});
    ExhaustiveAddAllStmt(type1, upper, type2, lower, true);
  }
}

void StmtStmtStore::AddCalls(bool is_star, std::string const &upper, std::string const &lower) {
  if (!is_star) {
    all_pairs.push_back({upper, lower});
    return;
  }

  all_star_pairs.push_back(std::pair<std::string, std::string>(upper, lower));
  ExhaustiveAddAllStmt(PROC, upper, PROC, lower, true);

  if (star_type_pair_map.find(PROC) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(PROC).find(PROC) != star_type_pair_map.at(PROC).end()) {
      for (auto &pair : star_type_pair_map.at(PROC).at(PROC).GetPairVector()) {
        if (upper == pair.second) {
          all_star_pairs.push_back({pair.first, lower});
          ExhaustiveAddAllStmt(PROC, pair.first, PROC, lower, true);
        }
        if (lower == pair.first) {
          all_star_pairs.push_back({upper, pair.second});
          ExhaustiveAddAllStmt(PROC, upper, PROC, pair.second, true);
        }
      }
    }
  }
}

void StmtStmtStore::ExhaustiveAddAllStmt(StmtType type1,
                                         std::string upper,
                                         StmtType type2,
                                         std::string lower,
                                         bool is_star) {
  NESTED_TUPLE_MAP *pair_map;

  if (!is_star) {
    pair_map = &type_pair_map;
  } else {
    pair_map = &star_type_pair_map;
  }

  ExhaustiveAddSubStmt(type1, upper, type2, lower, pair_map);
  ExhaustiveAddSubStmt(STMT, upper, type2, lower, pair_map);
  ExhaustiveAddSubStmt(type1, upper, STMT, lower, pair_map);
  ExhaustiveAddSubStmt(STMT, upper, STMT, lower, pair_map);
}

void StmtStmtStore::ExhaustiveAddSubStmt(StmtType type1,
                                         std::string upper,
                                         StmtType type2,
                                         std::string lower,
                                         NESTED_TUPLE_MAP *pair_map) {
  if (pair_map->find(type1) != pair_map->end()) {
    if (pair_map->at(type1).find(type2) != pair_map->at(type1).end()) {
      PopulatePairMap(type1, upper, type2, lower, pair_map);
    } else {
      PkbRelationship relationship;
      relationship.AddLowerSet(upper, {lower});
      relationship.AddUpperSet(lower, {upper});
      relationship.AddPair(upper, lower);
      pair_map->at(type1).insert({type2, relationship});
    }
  } else {
    PkbRelationship relationship;
    relationship.AddLowerSet(upper, {lower});
    relationship.AddUpperSet(lower, {upper});
    relationship.AddPair(upper, lower);
    pair_map->insert({type1, {}});
    pair_map->at(type1).insert({type2, relationship});
  }
}

void StmtStmtStore::PopulatePairMap(StmtType type1,
                                    std::string upper,
                                    StmtType type2,
                                    std::string lower,
                                    NESTED_TUPLE_MAP *pair_map) {
  if (pair_map->at(type1).at(type2).GetUpperToLowerSet().find(upper)
      != pair_map->at(type1).at(type2).GetUpperToLowerSet().end()) {
    pair_map->at(type1).at(type2).GetUpperToLowerSet().at(upper).insert(lower);
  } else {
    pair_map->at(type1).at(type2).AddLowerSet(upper, {lower});
  }

  if (pair_map->at(type1).at(type2).GetLowerToUpperSet().find(lower)
      != pair_map->at(type1).at(type2).GetLowerToUpperSet().end()) {
    pair_map->at(type1).at(type2).GetLowerToUpperSet().at(lower).insert(upper);
  } else {
    pair_map->at(type1).at(type2).GetLowerToUpperSet().insert({lower, {upper}});
  }

  pair_map->at(type1).at(type2).GetPairVector().push_back({upper, lower});
}

void StmtStmtStore::WipeNextStar() {
  all_star_pairs.clear();
  star_type_pair_map.clear();
}

bool StmtStmtStore::IsValid(IDENT_PAIR const &pair) {
  return std::find(all_pairs.begin(), all_pairs.end(), pair) != all_pairs.end();
}

bool StmtStmtStore::IsStarValid(IDENT_PAIR const &pair) {
  return std::find(all_star_pairs.begin(), all_star_pairs.end(), pair) != all_star_pairs.end();
}

bool StmtStmtStore::IsNextStarPairValid(std::pair<std::string, std::string> const &pair) {
  if (std::find(all_star_pairs.begin(), all_star_pairs.end(), pair) != all_star_pairs.end()) {
    return true;
  } else {
    std::unordered_set<std::string> next_star = GetLowerStarOf(NEXT, STMT, pair.first);
    return next_star.find(pair.second) != next_star.end();
  }
}

std::unordered_set<std::string> StmtStmtStore::GetHelper(StmtType type1,
                                                         StmtType type2,
                                                         int index,
                                                         std::string const &stmt,
                                                         bool is_star) {

  NESTED_TUPLE_MAP *pair_map;

  if (!is_star) {
    pair_map = &type_pair_map;
  } else {
    pair_map = &star_type_pair_map;
  }

  if (index == 0) {
    if (pair_map->at(type1).at(type2).GetUpperToLowerSet().find(stmt)
        != pair_map->at(type1).at(type2).GetUpperToLowerSet().end()) {
      return pair_map->at(type1).at(type2).GetUpperToLowerSet().at(stmt);
    }
  } else if (index == 1) {
    if (pair_map->at(type1).at(type2).GetLowerToUpperSet().find(stmt)
        != pair_map->at(type1).at(type2).GetLowerToUpperSet().end()) {
      return pair_map->at(type1).at(type2).GetLowerToUpperSet().at(stmt);
    }
  }
  return {};
}

std::string StmtStmtStore::GetUpperOf(StmtType stmt_type, std::string const &stmt) {
  if (type_pair_map.find(stmt_type) != type_pair_map.end()) {
    if (type_pair_map.at(stmt_type).find(STMT) != type_pair_map.at(stmt_type).end()) {
      return GetHelper(stmt_type, STMT, 1, stmt, false).empty() ? "0" : *(GetHelper(stmt_type,
                                                                                    STMT,
                                                                                    1,
                                                                                    stmt,
                                                                                    false).begin());
    }
  }
  return "0";
}

std::string StmtStmtStore::GetLowerOf(StmtType stmt_type, std::string const &stmt) {
  if (type_pair_map.find(STMT) != type_pair_map.end()) {
    if (type_pair_map.at(STMT).find(stmt_type) != type_pair_map.at(STMT).end()) {
      return GetHelper(STMT, stmt_type, 0, stmt, false).empty() ? "0" : *(GetHelper(STMT,
                                                                                    stmt_type,
                                                                                    0,
                                                                                    stmt,
                                                                                    false).begin());
    }
  }
  return "0";
}

std::unordered_set<std::string> StmtStmtStore::GetUpperSetOf(StoreType store_type,
                                                             StmtType stmt_type,
                                                             std::string const &stmt) {
  StmtType rs_type = PROC;
  if (store_type == NEXT) {
    rs_type = STMT;
  }

  if (type_pair_map.find(stmt_type) != type_pair_map.end()) {
    if (type_pair_map.at(stmt_type).find(rs_type) != type_pair_map.at(rs_type).end()) {
      return GetHelper(stmt_type, rs_type, 1, stmt, false);
    }
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerSetOf(StoreType store_type,
                                                             StmtType stmt_type,
                                                             std::string const &stmt) {
  std::unordered_set<std::string> result;

  if (type_pair_map.find(STMT) != type_pair_map.end()) {
    if (type_pair_map.at(STMT).find(stmt_type) != type_pair_map.at(STMT).end()) {
      return GetHelper(STMT, stmt_type, 0, stmt, false);
    }
  }

  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetUpperStarOf(StoreType store_type,
                                                              StmtType stmt_type,
                                                              std::string const &stmt) {

  if (store_type == NEXT) {
    if (star_type_pair_map.find(STMT) != star_type_pair_map.end()) {
      if (star_type_pair_map.at(STMT).find(stmt_type) != star_type_pair_map.at(STMT).end()) {
        if (star_type_pair_map.at(STMT).at(stmt_type).GetLowerToUpperSet().find(stmt)
            != star_type_pair_map.at(STMT).at(stmt_type).GetLowerToUpperSet().end()) {
          return GetHelper(STMT, stmt_type, 1, stmt, true);
        }
      }
    }
    std::unordered_set<std::string> res;
    std::unordered_set<std::string> visited;
    GetUpperStarOfHelper(stmt_type, stmt, res, visited);
    return res;
  }

  if (star_type_pair_map.find(stmt_type) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(stmt_type).find(STMT) != star_type_pair_map.at(stmt_type).end()) {
      return GetHelper(stmt_type, STMT, 1, stmt, true);
    }
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerStarOf(StoreType store_type,
                                                              StmtType stmt_type,
                                                              std::string const &stmt) {

  if (store_type == NEXT) {
    if (star_type_pair_map.find(STMT) != star_type_pair_map.end()) {
      if (star_type_pair_map.at(STMT).find(stmt_type) != star_type_pair_map.at(STMT).end()) {
        if (star_type_pair_map.at(STMT).at(stmt_type).GetUpperToLowerSet().find(stmt)
            != star_type_pair_map.at(STMT).at(stmt_type).GetUpperToLowerSet().end()) {
          return GetHelper(STMT, stmt_type, 0, stmt, true);
        }
      }
    }
    std::unordered_set<std::string> res;
    std::unordered_set<std::string> visited;
    GetLowerStarOfHelper(stmt_type, stmt, res, visited);
    return res;
  }

  if (star_type_pair_map.find(STMT) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(STMT).find(stmt_type) != star_type_pair_map.at(STMT).end()) {
      return GetHelper(STMT, stmt_type, 0, stmt, true);
    }
  }

  return {};
}

IDENT_PAIR_VECTOR StmtStmtStore::GetPairByType(StmtType type1, StmtType type2) {
  if (type_pair_map.find(type1) != type_pair_map.end()) {
    if (type_pair_map.at(type1).find(type2) != type_pair_map.at(type1).end()) {
      return type_pair_map.at(type1).at(type2).GetPairVector();
    }
  }
  return {};
}

IDENT_PAIR_VECTOR StmtStmtStore::GetStarPairByType(StmtType type1, StmtType type2) {
  if (star_type_pair_map.find(type1) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(type1).find(type2) != star_type_pair_map.at(type1).end()) {
      return star_type_pair_map.at(type1).at(type2).GetPairVector();
    }
  }
  return {};
}

void StmtStmtStore::GetUpperStarOfHelper(StmtType stmt_type,
                                         std::string const &stmt,
                                         std::unordered_set<std::string> &res,
                                         std::unordered_set<std::string> &visited) {
  if (visited.find(stmt) != visited.end()) {
    bool is_loop = false;
    for (auto next : visited) {
      if (next == stmt) {
        is_loop = true;
      }
      AddNext(true, stmt_type, stmt, STMT, next);
      if (is_loop) {
        AddNext(true, stmt_type, stmt, STMT, next);
      }
      res.insert(stmt);
    }
    return;
  }
  if (visited.size() > 0) {
    res.insert(stmt);
  }
  for (auto next : visited) {
    AddNext(true, stmt_type, stmt, STMT, next);
  }
  visited.insert(stmt);
  std::unordered_set<std::string> before_set = GetUpperSetOf(NEXT, STMT, stmt);
  for (auto before : before_set) {
    std::unordered_set<std::string> new_visited;
    for (auto stmt : visited) {
      new_visited.insert(stmt);
    }
    GetUpperStarOfHelper(stmt_type, before, res, new_visited);
  }
}

void StmtStmtStore::GetLowerStarOfHelper(StmtType stmt_type,
                                         std::string const &stmt,
                                         std::unordered_set<std::string> &res,
                                         std::unordered_set<std::string> &visited) {
  if (visited.find(stmt) != visited.end()) {
    bool is_loop = false;
    for (auto before : visited) {
      if (before == stmt) {
        is_loop = true;
      }
      AddNext(true, STMT, before, stmt_type, stmt);
      if (is_loop) {
        AddNext(true, STMT, before, stmt_type, stmt);
      }
      res.insert(stmt);
    }
    return;
  }
  if (visited.size() > 0) {
    res.insert(stmt);
  }
  for (auto before : visited) {
    AddNext(true, STMT, before, stmt_type, stmt);
  }
  visited.insert(stmt);
  std::unordered_set<std::string> next_set = GetLowerSetOf(NEXT, STMT, stmt);
  for (auto next : next_set) {
    std::unordered_set<std::string> new_visited;
    for (auto stmt : visited) {
      new_visited.insert(stmt);
    }
    GetLowerStarOfHelper(stmt_type, next, res, new_visited);
  }
}

IDENT_PAIR_VECTOR StmtStmtStore::GetAllPairs() {
  return all_pairs;
}

IDENT_PAIR_VECTOR StmtStmtStore::GetAllStarPairs() {
  return all_star_pairs;
}

IDENT_PAIR_VECTOR StmtStmtStore::GetAllNextStarPairs() {
  std::vector<std::pair<std::string, std::string>> res;
  for (auto pair : all_pairs) {
    std::string before_stmt = pair.first;
    std::string next_stmt = pair.second;
    std::unordered_set<std::string> lower_star = GetLowerStarOf(NEXT, STMT, before_stmt);
    for (auto next : lower_star) {
      res.push_back({pair.first, next});
    }
    std::unordered_set<std::string> upper_star = GetUpperStarOf(NEXT, STMT, next_stmt);
    for (auto before : upper_star) {
      res.push_back({before, pair.second});
    }
  }
  return res;
}