#include "modifies_store.h"

ModifiesStore::ModifiesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtVarStore(move(stmt_vector), move(stmt_type)) {}

ModifiesStore::IDENT_SET ModifiesStore::GetVarModByStmt(IDENT const &stmt) {
  return GetVarByStmt(stmt);
}

ModifiesStore::IDENT_SET ModifiesStore::GetStmtModByVar(StmtType type, IDENT const &var) {
  return GetStmtByVar(type, var);
}

ModifiesStore::IDENT_SET ModifiesStore::GetAllStmtModify() {
  return GetAllStmt();
}

ModifiesStore::IDENT_SET ModifiesStore::GetAllProcModify() {
  return GetAllProc();
}

ModifiesStore::IDENT_PAIR_SET ModifiesStore::GetAllModStmt(StmtType type) {
  return GetPairByType(type);
}
