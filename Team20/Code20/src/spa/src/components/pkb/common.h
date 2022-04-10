#ifndef COMMON_H
#define COMMON_H

#include <stack>
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



class PKB;
class ModifiesStore;
class UsesStore;
class AffectsStore;
class AffectsStoreFactory;

typedef std::shared_ptr<PKB> PkbPtr;
typedef std::shared_ptr<ModifiesStore> ModifiesStorePtr;
typedef std::shared_ptr<UsesStore> UsesStorePtr;
typedef std::shared_ptr<AffectsStore> AffectsStorePtr;
typedef std::shared_ptr<AffectsStoreFactory> AffectsStoreFactoryPtr;

typedef std::string IDENT;
typedef std::vector<IDENT> IDENT_VECTOR;
typedef std::unordered_set<IDENT> IDENT_SET;
typedef std::pair<IDENT, IDENT> IDENT_PAIR;
typedef std::unordered_set<IDENT_PAIR, pair_hash> IDENT_PAIR_SET;
typedef std::unordered_map<IDENT, IDENT_SET> IDENT_SET_MAP;
typedef std::shared_ptr<IDENT_SET_MAP> IDENT_SET_MAP_PTR;
typedef std::stack<IDENT_SET_MAP_PTR> IDENT_SET_MAP_PTR_STACK;
typedef std::vector<IDENT_SET> IDENT_SET_VECTOR;
typedef std::shared_ptr<IDENT_SET_VECTOR> IDENT_SET_VECTOR_PTR;
typedef std::unordered_map<IDENT, IDENT_PAIR> IDENT_PAIR_MAP;
typedef std::vector<IDENT_PAIR> IDENT_PAIR_VECTOR;
typedef std::unordered_map<StmtType,
                           std::unordered_map<StmtType,
                                              std::tuple<IDENT_SET_MAP,
                                                         IDENT_SET_MAP,
                                                         IDENT_PAIR_VECTOR>>> NESTED_TUPLE_MAP;

typedef std::shared_ptr<std::unordered_map<IDENT, StmtType>> IDENT_TO_STMT_TYPE_MAP_PTR;

#endif //COMMON_H
