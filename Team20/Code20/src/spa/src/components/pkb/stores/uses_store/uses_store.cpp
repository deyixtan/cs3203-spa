#include "uses_store.h"

UsesStore::UsesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector, std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtVarStore(move(stmt_vector), move(stmt_type)) {}

std::unordered_set<std::string> UsesStore::GetVarUsedByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> UsesStore::GetStmtUsedByVar(StmtType type, std::string const &var) {
  return GetStmtByVar(type, var);
}

std::unordered_set<std::string> UsesStore::GetAllStmtUsing() {
  return GetAllStmt();
}

std::unordered_set<std::string> UsesStore::GetAllProcUsing() {
  return GetAllProc();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> UsesStore::GetAllUsesStmt(StmtType type) {
  return GetPairByType(type);
}
