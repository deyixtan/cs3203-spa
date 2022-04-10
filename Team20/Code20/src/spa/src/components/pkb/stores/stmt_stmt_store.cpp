#include "stmt_stmt_store.h"
#include "../pkb_relationship.h"

StmtStmtStore::StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type)
    : Store(move(stmt_vector), move(stmt_type)) {}

void StmtStmtStore::ExhaustiveAddAllStmt(StmtType type1,
                                         std::string upper,
                                         StmtType type2,
                                         std::string lower,
                                         bool is_star) {
  NESTED_STMT_STMT_MAP *pair_map;

  if (!is_star) {
    pair_map = &type_pair_map; //std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);
  } else {
    pair_map = &star_type_pair_map; //std::make_shared<NESTED_STMT_STMT_MAP>(star_type_pair_map);
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
                                         NESTED_STMT_STMT_MAP *pair_map) {
  switch (IsMapContains(type1, type2, pair_map)) {
    case 1: // Both types exist
      PopulatePairMap(type1, upper, type2, lower, pair_map);
    case 2: // Only type 1 exists
      pair_map->at(type1).insert({type2, PkbRelationship()});
      PopulatePairMap(type1, upper, type2, lower, pair_map);
    case 3: // Both types missing
      pair_map->insert({type1, {}});
      pair_map->at(type1).insert({type2, PkbRelationship()});
      PopulatePairMap(type1, upper, type2, lower, pair_map);
  }
}

void StmtStmtStore::PopulatePairMap(StmtType type1,
                                    std::string upper,
                                    StmtType type2,
                                    std::string lower,
                                    NESTED_STMT_STMT_MAP *pair_map) {
  pair_map->at(type1).at(type2).AddLowerSet(upper, {lower});
  pair_map->at(type1).at(type2).AddUpperSet(lower, {upper});
  pair_map->at(type1).at(type2).AddPair(upper, lower);
}

bool StmtStmtStore::IsValid(IDENT_PAIR const &pair) {
  IDENT_PAIR_VECTOR vect = GetPairByType(STMT, STMT);
  return std::find(vect.begin(), vect.end(), pair) != vect.end();
}

bool StmtStmtStore::IsStarValid(IDENT_PAIR const &pair) {
  IDENT_PAIR_VECTOR vect = GetStarPairByType(STMT, STMT);
  return std::find(vect.begin(), vect.end(), pair) != vect.end();
}

IDENT_SET StmtStmtStore::GetHelper(StmtType type1,
                                   StmtType type2,
                                   int index,
                                   std::string const &stmt,
                                   bool is_star) {

  NESTED_STMT_STMT_MAP *pair_map;

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
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);

  if (IsMapContains(stmt_type, STMT, &type_pair_map) == 1) {
    return GetHelper(stmt_type, STMT, 1, stmt, false).empty() ? "0" : *(GetHelper(stmt_type,
                                                                                  STMT,
                                                                                  1,
                                                                                  stmt,
                                                                                  false).begin());
  }
  return "0";
}

std::string StmtStmtStore::GetLowerOf(StmtType stmt_type, std::string const &stmt) {
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);

  if (IsMapContains(STMT, stmt_type, &type_pair_map) == 1) {
    return GetHelper(STMT, stmt_type, 0, stmt, false).empty() ? "0" : *(GetHelper(STMT,
                                                                                  stmt_type,
                                                                                  0,
                                                                                  stmt,
                                                                                  false).begin());
  }
  return "0";
}

std::unordered_set<std::string> StmtStmtStore::GetUpperSetOf(StoreType store_type,
                                                             StmtType stmt_type,
                                                             std::string const &stmt) {
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);
  StmtType rs_type = PROC;

  if (store_type == NEXT) {
    rs_type = STMT;
  }

  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);

  if (IsMapContains(stmt_type, rs_type, &type_pair_map) == 1) {
    return GetHelper(stmt_type, rs_type, 1, stmt, false);
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerSetOf(StmtType stmt_type, std::string const &stmt) {
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);

  if (IsMapContains(STMT, stmt_type, &type_pair_map) == 1) {
    return GetHelper(STMT, stmt_type, 0, stmt, false);
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetUpperStarOf(StmtType stmt_type, std::string const &stmt) {

  if (star_type_pair_map.find(stmt_type) != star_type_pair_map.end()) {
    if (star_type_pair_map.at(stmt_type).find(STMT) != star_type_pair_map.at(stmt_type).end()) {
      return GetHelper(stmt_type, STMT, 1, stmt, true);
    }
  }
  return {};
}

std::unordered_set<std::string> StmtStmtStore::GetLowerStarOf(StmtType stmt_type, std::string const &stmt) {
  if (IsMapContains(STMT, stmt_type, &star_type_pair_map) == 1) {
    return GetHelper(STMT, stmt_type, 0, stmt, true);
  }
  return {};
}

IDENT_PAIR_VECTOR StmtStmtStore::GetPairByType(StmtType type1, StmtType type2) {
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(type_pair_map);

  if (IsMapContains(type1, type2, &type_pair_map) == 1) {
    return type_pair_map.at(type1).at(type2).GetPairVector();
  }
  return {};
}

IDENT_PAIR_VECTOR StmtStmtStore::GetStarPairByType(StmtType type1, StmtType type2) {
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(star_type_pair_map);

  if (IsMapContains(type1, type2, &star_type_pair_map) == 1) {
    return star_type_pair_map.at(type1).at(type2).GetPairVector();
  }
  return {};
}