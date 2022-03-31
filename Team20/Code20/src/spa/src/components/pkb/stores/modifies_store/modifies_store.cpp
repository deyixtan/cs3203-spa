#include "modifies_store.h"

ModifiesStore::ModifiesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtVarStore(move(stmt_vector), move(stmt_type)) {}

std::unordered_set<std::string> ModifiesStore::GetVarModByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> ModifiesStore::GetStmtModByVar(StmtType type, std::string const &var) {
  return GetStmtByVar(type, var);
}

std::unordered_set<std::string> ModifiesStore::GetAllStmtModify() {
  return GetAllStmt();
}

std::unordered_set<std::string> ModifiesStore::GetAllProcModify() {
  return GetAllProc();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ModifiesStore::GetAllModStmt(StmtType type) {
  return GetPairByType(type);
}
