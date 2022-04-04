#import "parent_store.h"

ParentStore::ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void ParentStore::AddParent(std::string const &parent, std::string const &child) {
  AddUpperLower(PARENT, parent, child);
}

void ParentStore::AddParentStar(std::string const &stmt, std::vector<std::string> const &visited) {
  AddUpperLowerStar(PARENT, "", stmt, visited);
}

bool ParentStore::IsParentPairValid(std::pair<std::string, std::string> const &pair) {
  return IsValid(pair);
}

bool ParentStore::IsAnceDescValid(std::pair<std::string, std::string> const &pair) {
  return IsStarValid(pair);
}

std::string ParentStore::GetParentOf(StmtType type, std::string const &stmt) {
  return GetUpperOf(type, stmt);
}

std::unordered_set<std::string> ParentStore::GetChildOf(StmtType type, std::string const &stmt) {
  return GetLowerSetOf(PARENT, type, stmt);
}

std::unordered_set<std::string> ParentStore::GetAllAnceOf(StmtType type, std::string const &stmt) {
  return GetUpperStarOf(PARENT, type, stmt);
}

std::unordered_set<std::string> ParentStore::GetAllDescOf(StmtType type, std::string const &stmt) {
  return GetLowerStarOf(PARENT, type, stmt);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetParentChildPairs() {
  return GetAllPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAnceDescPairs() {
  return GetAllStarPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStmt(StmtType type1,
                                                                                                 StmtType type2) {
  return GetPairByType(type1, type2);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStarStmt(StmtType type1,
                                                                                                     StmtType type2) {
  return GetStarPairByType(type1, type2);
}
