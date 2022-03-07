#include "modify_store.h"

ModifyStore::ModifyStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : StatementStore(move(stmt_vector)) {}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ModifyStore::GetAllModStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type, supported_types, GetAllStmtVar(), true);
}

std::unordered_set<std::string> ModifyStore::GetVarModByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> ModifyStore::GetStmtModByVar(std::string const &var) {
  return GetStmtByVar(var);
}

std::unordered_set<std::string> ModifyStore::GetVarModByProc(std::string const &proc) {
  return GetVarByProc(proc);
}

std::unordered_set<std::string> ModifyStore::GetProcModByVar(std::string const &var) {
  return GetProcByVar(var);
}

std::unordered_set<std::string> ModifyStore::GetAllStmtModify() {
  return GetAllStmt();
}

std::unordered_set<std::string> ModifyStore::GetAllProcModify() {
  return GetAllProc();
}
