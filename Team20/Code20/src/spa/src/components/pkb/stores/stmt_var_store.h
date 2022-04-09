#ifndef STATEMENT_STORE_H_
#define STATEMENT_STORE_H_

#include "store.h"

class StmtVarStore : public Store {
 private:
  IDENT_SET_MAP stmt_var_map;
  std::unordered_map<std::string, std::unordered_map<StmtType, std::unordered_set<std::string>>> var_stmt_map;
  std::unordered_map<StmtType, IDENT_PAIR_VECTOR> type_pair_map;
  IDENT_SET all_stmt;
  IDENT_SET all_proc;

 public:
  explicit StmtVarStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                        std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddStmtVar(IDENT stmt, IDENT var);
  void AddStmtVarHelper(StmtType type, IDENT stmt, IDENT var);
  [[nodiscard]] bool IsStmtVarValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT_SET GetVarByStmt(IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetStmtByVar(StmtType type, IDENT const &var);
  [[nodiscard]] IDENT_PAIR_VECTOR GetPairByType(StmtType type);
  [[nodiscard]] IDENT_SET GetAllStmt();
  [[nodiscard]] IDENT_SET GetAllProc();
};

#endif //STATEMENT_STORE_H_
