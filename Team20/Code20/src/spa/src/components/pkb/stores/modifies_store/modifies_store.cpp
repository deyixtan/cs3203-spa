#include "modifies_store.h"

ModifiesStore::ModifiesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StmtVarStore(move(stmt_vector)) {}

std::unordered_set<std::string> ModifiesStore::GetVarModByStmt(std::string const &stmt) {
  return GetVarByStmt(stmt);
}

std::unordered_set<std::string> ModifiesStore::GetStmtModByVar(StmtType type, std::string const &var) {
  return GetStmtByVar(type, var);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ModifiesStore::GetAllModStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type, supported_types, GetAllStmtVar(), true);
}
