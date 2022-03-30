#ifndef MODIFY_STORE_H
#define MODIFY_STORE_H

#include "components/pkb/stores/stmt_var_store.h"

// proxy class
class ModifiesStore : public StmtVarStore {
 public:
  explicit ModifiesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  [[nodiscard]] std::unordered_set<std::string> GetVarModByStmt(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtModByVar(StmtType type, std::string const &var);
  [[nodiscard]] std::unordered_set<std::string> GetAllStmtModify();
  [[nodiscard]] std::unordered_set<std::string> GetAllProcModify();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModStmt(StmtType type); //TODO: Update data structure
};

#endif //MODIFY_STORE_H
