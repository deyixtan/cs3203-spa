#ifndef STORE_H
#define STORE_H

#include "components/pkb/pkb_declarations.h"

namespace pkb {

class Store {
 protected:
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;
  std::shared_ptr<std::unordered_map<std::string, StmtType>> m_stmt_type;

 public:
  explicit Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                 std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  [[nodiscard]] int IsMapContains(StmtType type1, StmtType type2, NESTED_STMT_STMT_MAP *pair_map);
};

}

#endif //STORE_H
