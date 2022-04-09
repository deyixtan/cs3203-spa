#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>

#include "../pkb_declarations.h"
#include "../../../utils/pair_hash.h"

class Store {
 protected:
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;
  std::shared_ptr<std::unordered_map<std::string, StmtType>> m_stmt_type;

 public:
  explicit Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                 std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
