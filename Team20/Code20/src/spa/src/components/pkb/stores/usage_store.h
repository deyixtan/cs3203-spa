#ifndef USAGE_STORE_H
#define USAGE_STORE_H

#include "statement_store.h"

// proxy class
class UsageStore : public StatementStore {
 public:
  [[nodiscard]] std::unordered_set<std::string> GetVarUsedByStmt(std::string stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtUsedByVar(std::string var);
  [[nodiscard]] std::unordered_set<std::string> GetVarUsedByProc(std::string proc);
  [[nodiscard]] std::unordered_set<std::string> GetProcUsedByVar(std::string var);
  [[nodiscard]] std::unordered_set<std::string> GetAllStmtUsing();
  [[nodiscard]] std::unordered_set<std::string> GetAllProcUsing();
};

#endif //USAGE_STORE_H
