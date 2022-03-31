#import "parent_store.h"

ParentStore::ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StmtStmtStore(move(stmt_vector)) {}

void ParentStore::AddParent(std::string const &parent, std::string const &child) {
  AddUpperLower(PARENT, STMT, STMT, parent, child); //TODO: Integrate GetTypeOfStmt()
}

void ParentStore::AddParentStar(std::string const &stmt, std::vector<std::string> const &visited) {
  AddUpperLowerStar(PARENT, STMT, STMT, "", stmt, visited);
}

// Used for Parent(s1, s2)
bool ParentStore::ParentChildValid(std::pair<std::string, std::string> const &pair) {
  return IsValid(pair);
}

std::string ParentStore::GetParentOf(std::string const &stmt) {
  return GetUpperOf(PARENT, stmt);
}

std::unordered_set<std::string> ParentStore::GetChildOf(std::string const &stmt) {
  return GetLowerOf(PARENT, stmt);
}

std::unordered_set<std::string> ParentStore::GetAllAnceOf(std::string const &stmt) {
  return GetUpperStarOf(PARENT, stmt);
}

std::unordered_set<std::string> ParentStore::GetAllDescOf(std::string const &stmt) {
  return GetLowerStarOf(PARENT, stmt);
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
