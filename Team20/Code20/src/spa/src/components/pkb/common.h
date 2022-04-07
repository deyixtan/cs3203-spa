#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../../utils/pair_hash.h"

enum StmtType {
  STMT,
  READ,
  PRINT,
  WHILE,
  IF,
  ASSIGN,
  VARS,
  CONSTS,
  PROC,
  CALL,
  COUNT = CALL + 1 // get max value of StmtType
};

enum StoreType {
  USES,
  MODIFIES,
  FOLLOWS,
  PARENT,
  CALLS,
  NEXT,
  AFFECTS
};

typedef std::string IDENT;
typedef std::vector<IDENT> IDENT_VECTOR;
typedef std::unordered_set<IDENT> IDENT_SET;
typedef std::pair<IDENT, IDENT> IDENT_PAIR;
typedef std::unordered_map<IDENT, IDENT_SET> IDENT_SET_MAP;
typedef std::unordered_map<IDENT, IDENT_PAIR> IDENT_PAIR_MAP;
typedef std::unordered_set<IDENT_PAIR, pair_hash> IDENT_PAIR_SET;
typedef std::unordered_map<StmtType,
                           std::unordered_map<StmtType,
                                              std::tuple<IDENT_SET_MAP,
                                                         IDENT_SET_MAP,
                                                         IDENT_PAIR_SET>>> NESTED_TUPLE_MAP;

#endif //COMMON_H
