#ifndef STMT_VAR_STORE_H
#define STMT_VAR_STORE_H

#include "components/pkb/pkb_declarations.h"
#include "store.h"

namespace pkb {

class StmtVarStore : public Store {
 private:
  IDENT_SET_MAP stmt_var_map;
  NESTED_VAR_STMT_MAP var_stmt_map;
  IDENT_PAIR_VECTOR_MAP type_pair_map;
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
  [[nodiscard]] IDENT_SET GetAllStmt();
  [[nodiscard]] IDENT_SET GetAllProc();
  [[nodiscard]] IDENT_PAIR_VECTOR GetPairByType(StmtType type);
};

}

#endif //STMT_VAR_STORE_H
