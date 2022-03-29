#import "parent_store.h"

ParentStore::ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StmtStmtStore(move(stmt_vector)) {}

void ParentStore::AddParent(std::string const &parent, std::string const &child) {
  AddUpperLower(PARENT, parent, child);
}

void ParentStore::AddParentStar(std::string const &stmt, std::vector<std::string> const &visited) {
  AddUpperLowerStar(PARENT, "", stmt, visited);
}

bool ParentStore::IsParent(std::string const &stmt) {
  return IsUpper(stmt);
}

bool ParentStore::IsChild(std::string const &stmt) {
  return IsLower(stmt);
}

bool ParentStore::IsAnce(std::string const &stmt) {
  return IsUpperStar(stmt);
}

bool ParentStore::IsDesc(std::string const &stmt) {
  return IsLowerStar(stmt);
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

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return GetAllStmt(type, supported_types, GetParentChildPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStmt(StmtType type1,
                                                                                                 StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, WHILE, IF};
  return GetAllStmt(type1, type2, supported_types, GetAllParentStmt(type2), true);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStarStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return GetAllStmt(type, supported_types, GetAnceDescPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStarStmt(StmtType type1,
                                                                                                     StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, WHILE, IF};
  return GetAllStmt(type1, type2, supported_types, GetAllParentStarStmt(type2), true);
}
