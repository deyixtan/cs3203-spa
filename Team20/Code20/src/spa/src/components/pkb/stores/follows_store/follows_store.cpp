#include "follows_store.h"

FollowsStore::FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                           std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void FollowsStore::AddFollow(IDENT const &follower, IDENT const &following) {
  AddUpperLower(FOLLOWS, follower, following);
}

void FollowsStore::AddFollowStar(IDENT const &follower, IDENT const &following) {
  AddUpperLowerStar(FOLLOWS, follower, following, std::vector<std::string>());
}

bool FollowsStore::IsFollowsPairValid(IDENT_PAIR const &pair) {
  return IsValid(pair);
}

bool FollowsStore::IsFollowsStarPairValid(IDENT_PAIR const &pair) {
  return IsStarValid(pair);
}

FollowsStore::IDENT FollowsStore::GetFollowerOf(StmtType type, IDENT const &stmt) {
  return GetUpperOf(type, stmt);
}

FollowsStore::IDENT FollowsStore::GetFollowingOf(StmtType type, IDENT const &stmt) {
  return GetLowerOf(type, stmt);
}

FollowsStore::IDENT_SET FollowsStore::GetFollowerStarOf(StmtType type, IDENT const &stmt) {
  return GetUpperStarOf(FOLLOWS, type, stmt);
}

FollowsStore::IDENT_SET FollowsStore::GetFollowingStarOf(StmtType type, IDENT const &stmt) {
  return GetLowerStarOf(FOLLOWS, type, stmt);
}

FollowsStore::IDENT_PAIR_SET FollowsStore::GetFollowPairs() {
  return GetAllPairs();
}

FollowsStore::IDENT_PAIR_SET FollowsStore::GetFollowStarPairs() {
  return GetAllStarPairs();
}

FollowsStore::IDENT_PAIR_SET FollowsStore::GetAllFollowStmt(StmtType type1, StmtType type2) {
  return GetPairByType(type1, type2);
}

FollowsStore::IDENT_PAIR_SET FollowsStore::GetAllFollowStarStmt(StmtType type1, StmtType type2) {
  return GetStarPairByType(type1, type2);
}
