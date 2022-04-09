#include "store.h"

Store::Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type)
    : m_stmt_vector(move(stmt_vector)), m_stmt_type(move(stmt_type)) {}

int Store::IsMapContains(StmtType type1, StmtType type2, NESTED_STMT_STMT_MAP *pair_map) {
  if (pair_map->find(type1) != pair_map->end()) {
    if (pair_map->at(type1).find(type2) != pair_map->at(type1).end()) {
      return 1; // Both types exist
    } else {
      return 2; // Only type 1 exists
    }
  } else {
    return 3; // Both types missing
  }
}