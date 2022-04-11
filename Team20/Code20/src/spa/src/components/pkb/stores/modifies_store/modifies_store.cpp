#include "modifies_store.h"

namespace pkb {

ModifiesStore::ModifiesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtVarStore(move(stmt_vector), move(stmt_type)) {}

IDENT_SET ModifiesStore::GetVarModByStmt(IDENT const &stmt) {
  return GetVarByStmt(stmt);
}

IDENT_SET ModifiesStore::GetStmtModByVar(StmtType type, IDENT const &var) {
  return GetStmtByVar(type, var);
}

IDENT_SET ModifiesStore::GetAllStmtModify() {
  return GetAllStmt();
}

IDENT_SET ModifiesStore::GetAllProcModify() {
  return GetAllProc();
}

IDENT_PAIR_VECTOR ModifiesStore::GetAllModStmt(StmtType type) {
  return GetPairByType(type);
}

}
