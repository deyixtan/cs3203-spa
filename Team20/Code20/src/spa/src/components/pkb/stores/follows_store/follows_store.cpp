#include "follows_store.h"
#include "../../pkb_relationship.h"

FollowsStore::FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                           std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void FollowsStore::AddFollow(IDENT const &follower, IDENT const &following) {
  //AddUpperLower(FOLLOWS, follower, following);
  all_pairs.push_back({follower, following});
  ExhaustiveAddAllStmt(m_stmt_type->at(follower), follower, m_stmt_type->at(following), following, false);
}

void FollowsStore::AddFollowStar(IDENT const &follower, IDENT const &following) {
  //AddUpperLowerStar(FOLLOWS, follower, following, std::vector<std::string>());
  all_star_pairs.push_back({follower, following});
  ExhaustiveAddAllStmt(m_stmt_type->at(follower), follower, m_stmt_type->at(following), following, true);
}

bool FollowsStore::IsFollowsPairValid(IDENT_PAIR const &pair) {
  return IsValid(pair);
}

bool FollowsStore::IsFollowsStarPairValid(IDENT_PAIR const &pair) {
  return IsStarValid(pair);
}

IDENT FollowsStore::GetFollowerOf(StmtType type, IDENT const &stmt) {
  return GetUpperOf(type, stmt);
}

IDENT FollowsStore::GetFollowingOf(StmtType type, IDENT const &stmt) {
  return GetLowerOf(type, stmt);
}

IDENT_SET FollowsStore::GetFollowerStarOf(StmtType type, IDENT const &stmt) {
  return GetUpperStarOf(FOLLOWS, type, stmt);
}

IDENT_SET FollowsStore::GetFollowingStarOf(StmtType type, IDENT const &stmt) {
  return GetLowerStarOf(FOLLOWS, type, stmt);
}

IDENT_PAIR_VECTOR FollowsStore::GetAllFollowStmt(StmtType type1, StmtType type2) {
  return GetPairByType(type1, type2);
}

IDENT_PAIR_VECTOR FollowsStore::GetAllFollowStarStmt(StmtType type1, StmtType type2) {
  return GetStarPairByType(type1, type2);
}
