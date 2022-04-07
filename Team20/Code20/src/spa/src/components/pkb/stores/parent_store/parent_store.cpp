#import "parent_store.h"

ParentStore::ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void ParentStore::AddParent(IDENT const &parent, IDENT const &child) {
  AddUpperLower(PARENT, parent, child);
}

void ParentStore::AddParentStar(IDENT const &stmt, IDENT_VECTOR const &visited) {
  AddUpperLowerStar(PARENT, "", stmt, visited);
}

bool ParentStore::IsParentPairValid(IDENT_PAIR const &pair) {
  return IsValid(pair);
}

bool ParentStore::IsAnceDescValid(IDENT_PAIR const &pair) {
  return IsStarValid(pair);
}

IDENT ParentStore::GetParentOf(StmtType type, IDENT const &stmt) {
  return GetUpperOf(type, stmt);
}

IDENT_SET ParentStore::GetChildOf(StmtType type, IDENT const &stmt) {
  return GetLowerSetOf(PARENT, type, stmt);
}

IDENT_SET ParentStore::GetAllAnceOf(StmtType type, IDENT const &stmt) {
  return GetUpperStarOf(PARENT, type, stmt);
}

IDENT_SET ParentStore::GetAllDescOf(StmtType type, IDENT const &stmt) {
  return GetLowerStarOf(PARENT, type, stmt);
}

IDENT_PAIR_SET ParentStore::GetParentChildPairs() {
  return GetAllPairs();
}

IDENT_PAIR_SET ParentStore::GetAnceDescPairs() {
  return GetAllStarPairs();
}

IDENT_PAIR_SET ParentStore::GetAllParentStmt(StmtType type1, StmtType type2) {
  return GetPairByType(type1, type2);
}

IDENT_PAIR_SET ParentStore::GetAllParentStarStmt(StmtType type1, StmtType type2) {
  return GetStarPairByType(type1, type2);
}
