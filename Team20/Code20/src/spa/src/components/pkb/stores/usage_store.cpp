#include "usage_store.h"

UsageStore::UsageStore(std::vector<std::unordered_set<std::string>> &stmt_vector) : StatementStore(stmt_vector) {}

std::unordered_set<std::string> UsageStore::GetVarUsedByStmt(std::string stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> UsageStore::GetStmtUsedByVar(std::string var) {
  return GetStmtByVar(var);
}

std::unordered_set<std::string> UsageStore::GetVarUsedByProc(std::string proc) {
  return GetVarByProc(proc);
}

std::unordered_set<std::string> UsageStore::GetProcUsedByVar(std::string var) {
  return GetProcByVar(var);
}

std::unordered_set<std::string> UsageStore::GetAllStmtUsing() {
  return GetAllStmt();
}

std::unordered_set<std::string> UsageStore::GetAllProcUsing() {
  return GetAllProc();
}
