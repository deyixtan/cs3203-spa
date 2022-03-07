#ifndef USAGE_STORE_H
#define USAGE_STORE_H

#include "statement_store.h"

// proxy class
class UsageStore : public StatementStore {
 public:
  explicit UsageStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsesStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::string> GetVarUsedByStmt(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtUsedByVar(std::string const &var);
  [[nodiscard]] std::unordered_set<std::string> GetVarUsedByProc(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetProcUsedByVar(std::string const &var);
  [[nodiscard]] std::unordered_set<std::string> GetAllStmtUsing();
  [[nodiscard]] std::unordered_set<std::string> GetAllProcUsing();
};

#endif //USAGE_STORE_H
