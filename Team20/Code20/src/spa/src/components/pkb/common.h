#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../../utils/pair_hash.h"

class PkbRelationship;

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

class PKB;
typedef std::shared_ptr<PKB> PkbPtr;

typedef std::string IDENT;
typedef std::vector<IDENT> IDENT_VECTOR;
typedef std::unordered_set<IDENT> IDENT_SET;
typedef std::pair<IDENT, IDENT> IDENT_PAIR;
typedef std::unordered_map<IDENT, IDENT_SET> IDENT_SET_MAP;
typedef std::unordered_map<IDENT, IDENT_PAIR> IDENT_PAIR_MAP;
typedef std::vector<IDENT_PAIR> IDENT_PAIR_VECTOR;
typedef std::unordered_map<StmtType, std::unordered_map<StmtType, PkbRelationship>> NESTED_TUPLE_MAP;

#endif //COMMON_H
