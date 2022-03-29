#include "uses_store.h"

UsesStore::UsesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StmtVarStore(move(stmt_vector)) {}

std::unordered_set<std::string> UsesStore::GetVarUsedByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> UsesStore::GetStmtUsedByVar(std::string const &var) {
  return GetStmtByVar(var);
}

std::unordered_set<std::string> UsesStore::GetVarUsedByProc(std::string const &proc) {
  return GetVarByProc(proc);
}

std::unordered_set<std::string> UsesStore::GetProcUsedByVar(std::string const &var) {
  return GetProcByVar(var);
}

std::unordered_set<std::string> UsesStore::GetAllStmtUsing() {
  return GetAllStmt();
}

std::unordered_set<std::string> UsesStore::GetAllProcUsing() {
  return GetAllProc();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> UsesStore::GetAllUsesStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, PRINT, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type, supported_types, GetAllStmtVar(), true);
}