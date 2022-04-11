#import "parent_store.h"
#include "../../pkb_relationship.h"

ParentStore::ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void ParentStore::AddParent(IDENT const &parent, IDENT const &child) {
  ExhaustiveAddAllStmt(m_stmt_type->at(parent), parent, m_stmt_type->at(child), child, false);
}

void ParentStore::AddParentStar(IDENT const &stmt, IDENT_VECTOR const &visited) {
  for (std::string const &ance : visited) {
    if (ance == stmt) {
      continue;
    }

    if (ance != "") {
      ExhaustiveAddAllStmt(m_stmt_type->at(ance), ance, m_stmt_type->at(stmt), stmt, true);
    }
  }
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
  return GetLowerSetOf(type, stmt);
}

IDENT_SET ParentStore::GetAllAnceOf(StmtType type, IDENT const &stmt) {
  return GetUpperStarOf(type, stmt);
}

IDENT_SET ParentStore::GetAllDescOf(StmtType type, IDENT const &stmt) {
  return GetLowerStarOf(type, stmt);
}

IDENT_PAIR_VECTOR ParentStore::GetAllParentStmt(StmtType type1, StmtType type2) {
  return GetPairByType(type1, type2);
}

IDENT_PAIR_VECTOR ParentStore::GetAllParentStarStmt(StmtType type1, StmtType type2) {
  return GetStarPairByType(type1, type2);
}
