#ifndef STATEMENT_STORE_H_
#define STATEMENT_STORE_H_

#include "store.h"

class StmtVarStore : public Store {
 private:
  std::unordered_map<std::string, std::unordered_set<std::string>> stmt_var_map;
  std::unordered_map<std::string, std::unordered_map<StmtType, std::unordered_set<std::string>>> var_stmt_map;
  std::unordered_map<StmtType, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> type_pair_map;
  std::unordered_set<std::string> all_stmt;
  std::unordered_set<std::string> all_proc;

 public:
  explicit StmtVarStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddStmtVar(StmtType type, std::string stmt, std::string var);
  [[nodiscard]] bool IsStmtVarValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetVarByStmt(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetStmtByVar(StmtType type, std::string const &var);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetPairByType(StmtType type);
  [[nodiscard]] std::unordered_set<std::string> GetAllStmt();
  [[nodiscard]] std::unordered_set<std::string> GetAllProc();
};

#endif //STATEMENT_STORE_H_
