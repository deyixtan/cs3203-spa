#include "usage_store.h"

UsageStore::UsageStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StatementStore(move(stmt_vector)) {}

std::unordered_set<std::string> UsageStore::GetVarUsedByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> UsageStore::GetStmtUsedByVar(std::string const &var) {
  return GetStmtByVar(var);
}

std::unordered_set<std::string> UsageStore::GetVarUsedByProc(std::string const &proc) {
  return GetVarByProc(proc);
}

std::unordered_set<std::string> UsageStore::GetProcUsedByVar(std::string const &var) {
  return GetProcByVar(var);
}

std::unordered_set<std::string> UsageStore::GetAllStmtUsing() {
  return GetAllStmt();
}

std::unordered_set<std::string> UsageStore::GetAllProcUsing() {
  return GetAllProc();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> UsageStore::GetAllUsesStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, PRINT, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type, supported_types, GetAllStmtVar(), true);
}