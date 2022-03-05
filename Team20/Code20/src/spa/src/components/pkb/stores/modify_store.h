#ifndef MODIFY_STORE_H
#define MODIFY_STORE_H

#include "statement_store.h"

// proxy class
class ModifyStore : public StatementStore {
 public:
  [[nodiscard]] std::unordered_set<std::string> GetVarModByStmt(std::string stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtModByVar(std::string var);
  [[nodiscard]] std::unordered_set<std::string> GetVarModByProc(std::string proc);
  [[nodiscard]] std::unordered_set<std::string> GetProcModByVar(std::string var);
  [[nodiscard]] std::unordered_set<std::string> GetAllStmtModify();
  [[nodiscard]] std::unordered_set<std::string> GetAllProcModify();
};

#endif //MODIFY_STORE_H
