#include "uses_store.h"

UsesStore::UsesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector, std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtVarStore(move(stmt_vector), move(stmt_type)) {}

UsesStore::IDENT_SET UsesStore::GetVarUsedByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

UsesStore::IDENT_SET UsesStore::GetStmtUsedByVar(StmtType type, std::string const &var) {
  return GetStmtByVar(type, var);
}

UsesStore::IDENT_SET UsesStore::GetAllStmtUsing() {
  return GetAllStmt();
}

UsesStore::IDENT_SET UsesStore::GetAllProcUsing() {
  return GetAllProc();
}

UsesStore::IDENT_PAIR_SET UsesStore::GetAllUsesStmt(StmtType type) {
  return GetPairByType(type);
}
