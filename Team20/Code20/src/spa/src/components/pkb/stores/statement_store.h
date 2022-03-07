#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STATEMENT_STORE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STATEMENT_STORE_H_

#include "store.h"

class StatementStore : public Store {
 private:
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> stmt_var_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> proc_var_pairs;
  std::unordered_map<std::string, std::unordered_set<std::string>> stmt_var_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> var_stmt_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> proc_var_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> var_proc_map;
  std::unordered_set<std::string> all_stmt;
  std::unordered_set<std::string> all_proc;

 public:
  explicit StatementStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddStmtVar(std::string stmt, std::string var);
  void AddProcVar(std::string proc, std::string var);
  [[nodiscard]] bool StmtVarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool ProcVarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetVarByStmt(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtByVar(std::string const &var);
  [[nodiscard]] std::unordered_set<std::string> GetVarByProc(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetProcByVar(std::string const &var);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmtVar();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllProcVar();
  [[nodiscard]] std::unordered_set<std::string> GetAllStmt();
  [[nodiscard]] std::unordered_set<std::string> GetAllProc();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_STATEMENT_STORE_H_
