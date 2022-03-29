#include "follows_store.h"

FollowsStore::FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StmtStmtStore(move(stmt_vector)) {}

void FollowsStore::AddFollow(std::string const &follower, std::string const &following) {
  AddUpperLower(FOLLOWS, follower, following);
}

void FollowsStore::AddFollowStar(std::string const &follower, std::string const &following) {
  AddUpperLowerStar(FOLLOWS, follower, following, std::vector<std::string>());
}

bool FollowsStore::IsFollower(std::string const &stmt) {
  return IsUpper(stmt);
}

bool FollowsStore::IsFollowing(std::string const &stmt) {
  return IsLower(stmt);
}

bool FollowsStore::IsFollowerStar(std::string const &stmt) {
  return IsUpperStar(stmt);
}

bool FollowsStore::IsFollowingStar(std::string const &stmt) {
  return IsLowerStar(stmt);
}

// Used for follower(s1, s2)
bool FollowsStore::IsFollowExists(std::pair<std::string, std::string> const &pair) {
  return IsExists(pair);
}

// Used for follower*(s1, s2)
bool FollowsStore::IsFollowStarExists(std::pair<std::string, std::string> const &pair) {
  return IsStarExists(pair);
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

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetAllFollowStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type, supported_types, GetFollowPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetAllFollowStmt(StmtType type1,
                                                                                                 StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllFollowStmt(type2), true);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetAllFollowStarStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type, supported_types, GetFollowStarPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsStore::GetAllFollowStarStmt(StmtType type1,
                                                                                                     StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllFollowStarStmt(type2), true);
}
