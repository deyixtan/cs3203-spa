#include "follows_store.h"

FollowsStore::FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                           std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void FollowsStore::AddFollow(std::string const &follower,
                             std::string const &following) {
  AddUpperLower(FOLLOWS, follower, following);
}

void FollowsStore::AddFollowStar(std::string const &follower,
                                 std::string const &following) {
  AddUpperLowerStar(FOLLOWS, follower, following, std::vector<std::string>());
}

// Used for follower(s1, s2)
bool FollowsStore::IsFollowValid(std::pair<std::string, std::string> const &pair) {
  return IsValid(pair);
}

// Used for follower*(s1, s2)
bool FollowsStore::IsFollowStarValid(std::pair<std::string, std::string> const &pair) {
  return IsStarValid(pair);
}

std::string FollowsStore::GetFollowerOf(std::string const &stmt) {
  return GetUpperOf(FOLLOWS, stmt);
}

std::string FollowsStore::GetFollowingOf(std::string const &stmt) {
  return GetLowerOf(stmt);
}

std::unordered_set<std::string> FollowsStore::GetFollowerStarOf(std::string const &stmt) {
  return GetUpperStarOf(FOLLOWS, stmt);
}

std::unordered_set<std::string> FollowsStore::GetFollowingStarOf(std::string const &stmt) {
  return GetLowerStarOf(FOLLOWS, stmt);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetFollowPairs() {
  return GetAllPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetFollowStarPairs() {
  return GetAllStarPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetAllFollowStmt(StmtType type1,
                                                                                                  StmtType type2) {
  return GetPairByType(type1, type2);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetAllFollowStarStmt(StmtType type1,
                                                                                                      StmtType type2) {
  return GetStarPairByType(type1, type2);
}
