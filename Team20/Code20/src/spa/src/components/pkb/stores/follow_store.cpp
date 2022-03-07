#include "follow_store.h"

FollowStore::FollowStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    Store(move(stmt_vector)) {}

void FollowStore::AddFollow(std::string const &follower, std::string const &following) {
  AddFollowHelper(false, follower, following);
}

void FollowStore::AddFollowStar(std::string const &follower, std::string const &following) {
  AddFollowHelper(true, follower, following);
}

void FollowStore::AddFollowHelper(bool is_star, std::string const &follower, std::string const &following) {
  if (rs_map.find(follower) == rs_map.end()) {
    rs_map.insert({follower, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (rs_map.find(following) == rs_map.end()) {
    rs_map.insert({following, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (is_star) {
    rs_map.at(follower).following_star.insert(following);
    rs_map.at(following).follower_star.insert(follower);
    follower_star_set.insert(follower);
    following_star_set.insert(following);
    all_follow_star_pairs.emplace(std::pair<std::string, std::string>(follower, following));
    return;
  }
  rs_map.at(follower).following = following;
  rs_map.at(following).follower = follower;
  follower_set.insert(follower);
  following_set.insert(following);
  all_follow_pairs.emplace(std::pair<std::string, std::string>(follower, following));
}

bool FollowStore::IsFollower(std::string const &stmt) {
  return follower_set.find(stmt) != follower_set.end();
}

bool FollowStore::IsFollowing(std::string const &stmt) {
  return following_set.find(stmt) != following_set.end();
}

bool FollowStore::IsFollowerStar(std::string const &stmt) {
  return follower_star_set.find(stmt) != follower_star_set.end();
}

bool FollowStore::IsFollowingStar(std::string const &stmt) {
  return following_star_set.find(stmt) != following_star_set.end();
}

// Used for follower(s1, s2)
bool FollowStore::FollowExists(std::pair<std::string, std::string> const &pair) {
  return all_follow_pairs.find(pair) != all_follow_pairs.end();
}

// Used for follower*(s1, s2)
bool FollowStore::FollowStarExists(std::pair<std::string, std::string> const &pair) {
  return all_follow_star_pairs.find(pair) != all_follow_star_pairs.end();
}

std::string FollowStore::GetFollowerOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).follower;
  }
  return "0";
}

std::string FollowStore::GetFollowingOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).following;
  }
  return "0";
}

std::unordered_set<std::string> FollowStore::GetFollowerStarOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).follower_star;
  }
  return {};
}

std::unordered_set<std::string> FollowStore::GetFollowingStarOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).following_star;
  }
  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowStore::GetFollowPairs() {
  return all_follow_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowStore::GetFollowStarPairs() {
  return all_follow_star_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowStore::GetAllFollowStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type, supported_types, GetFollowPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowStore::GetAllFollowStmt(StmtType type1,
                                                                                                 StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllFollowStmt(type2), true);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowStore::GetAllFollowStarStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type, supported_types, GetFollowStarPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowStore::GetAllFollowStarStmt(StmtType type1,
                                                                                                     StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN};
  return Store::GetAllStmt(type1, type2, supported_types, GetAllFollowStarStmt(type2), true);
}
