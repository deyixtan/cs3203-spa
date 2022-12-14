#ifndef PKB_DECLARATIONS_H
#define PKB_DECLARATIONS_H

#include "components/common_declarations.h"
#include "utils/pair_hash.h"

namespace pkb {

enum StoreType {
  USES,
  CALLS,
  NEXT,
  AFFECTS
};

class PKB;
class PkbClient;
class ModifiesStore;
class UsesStore;
class FollowsStore;
class PatternStore;
class ParentStore;
class CallStore;
class NextStore;
class AffectsStore;
class AffectsStoreFactory;
class PkbRelationship;

typedef std::shared_ptr<PKB> PkbPtr;
typedef std::shared_ptr<PkbClient> PkbClientPtr;
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
typedef std::unordered_map<StmtType, IDENT_PAIR_VECTOR> IDENT_PAIR_VECTOR_MAP;
typedef std::unordered_map<std::string, std::unordered_map<StmtType, IDENT_SET>> NESTED_VAR_STMT_MAP;
typedef std::unordered_map<StmtType, std::unordered_map<StmtType, PkbRelationship>> NESTED_STMT_STMT_MAP;
typedef std::shared_ptr<NESTED_STMT_STMT_MAP> NESTED_STMT_STMT_MAP_PTR;
typedef std::shared_ptr<std::unordered_map<IDENT, StmtType>> IDENT_TO_STMT_TYPE_MAP_PTR;

}

#endif //PKB_DECLARATIONS_H
