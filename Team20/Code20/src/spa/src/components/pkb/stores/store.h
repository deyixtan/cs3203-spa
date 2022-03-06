#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

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
  COUNT = CALL // get max value of StmtType
};

class Store {
 protected:
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;

 public:
  Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmt(StmtType type,
                                                                                std::vector<StmtType> &supported_types,
                                                                                std::unordered_set<std::pair<std::string,
                                                                                                             std::string>,
                                                                                                   pair_hash> list,
                                                                                bool checkPairFirst);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmt(StmtType type1,
                                                                                StmtType type2,
                                                                                std::vector<StmtType> &supported_types,
                                                                                std::unordered_set<std::pair<std::string,
                                                                                                             std::string>,
                                                                                                   pair_hash> list,
                                                                                bool checkPairFirst);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STORE_H_
