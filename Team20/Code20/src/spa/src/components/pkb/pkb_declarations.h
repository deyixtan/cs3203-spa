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
typedef std::unordered_map<StmtType, std::unordered_map<StmtType, PkbRelationship>> NESTED_STMT_STMT_MAP;
typedef std::shared_ptr<NESTED_STMT_STMT_MAP> NESTED_STMT_STMT_MAP_PTR;

template <typename Key, typename T>
struct Hashmap {
  typedef std::unordered_map<Key, T> type;
};

#endif //COMMON_H
