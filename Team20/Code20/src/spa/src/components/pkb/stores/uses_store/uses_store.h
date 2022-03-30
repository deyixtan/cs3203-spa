#ifndef USAGE_STORE_H
#define USAGE_STORE_H

#include "components/pkb/stores/stmt_var_store.h"

// proxy class
class UsesStore : public StmtVarStore {
 public:
  explicit UsesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  [[nodiscard]] std::unordered_set<std::string> GetVarUsedByStmt(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtUsedByVar(StmtType type, std::string const &var);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsesStmt(StmtType type);
};

#endif //USAGE_STORE_H
