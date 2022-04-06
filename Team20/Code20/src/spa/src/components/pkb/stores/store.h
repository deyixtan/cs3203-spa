#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>

#include "../../../utils/pair_hash.h"

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

class Store {
 protected:
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;
  std::shared_ptr<std::unordered_map<std::string, StmtType>> m_stmt_type;

 public:
  explicit Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector, std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmt(StmtType type,
                                                                                std::vector<StmtType> &supported_types,
                                                                                std::unordered_set<std::pair<std::string,
                                                                                                             std::string>,
                                                                                                   pair_hash> const &list,
                                                                                bool checkPairFirst);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmt(StmtType type1,
                                                                                StmtType type2,
                                                                                std::vector<StmtType> &supported_types,
                                                                                std::unordered_set<std::pair<std::string,
                                                                                                             std::string>,
                                                                                                   pair_hash> const &list,
                                                                                bool checkPairFirst);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
