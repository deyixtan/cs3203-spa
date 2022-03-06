#include "modify_store.h"

ModifyStore::ModifyStore(std::vector<std::unordered_set<std::string>> &stmt_vector) : StatementStore(stmt_vector) {}

std::unordered_set<std::string> ModifyStore::GetVarModByStmt(std::string stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> ModifyStore::GetStmtModByVar(std::string var) {
  return GetStmtByVar(var);
}

std::unordered_set<std::string> ModifyStore::GetVarModByProc(std::string proc) {
  return GetVarByProc(proc);
}

std::unordered_set<std::string> ModifyStore::GetProcModByVar(std::string var) {
  return GetProcByVar(var);
}

std::unordered_set<std::string> ModifyStore::GetAllStmtModify() {
  return GetAllStmt();
}

std::unordered_set<std::string> ModifyStore::GetAllProcModify() {
  return GetAllProc();
}
