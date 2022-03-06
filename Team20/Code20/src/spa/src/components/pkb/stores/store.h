#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../../../utils/pair_hash.h"

class Store {
 protected:
  std::vector<std::unordered_set<std::string>> m_stmt_vector;

 public:
  Store(std::vector<std::unordered_set<std::string>> &stmt_vector);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
