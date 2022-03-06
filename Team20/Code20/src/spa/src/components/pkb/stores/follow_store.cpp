#include "follow_store.h"

FollowStore::FollowStore(std::vector<std::unordered_set<std::string>> &stmt_vector) : Store(stmt_vector) {}

bool FollowStore::IsFollower(std::string stmt) {
  return follower_set.find(stmt) != follower_set.end();
}

bool FollowStore::IsFollowing(std::string stmt) {
  return following_set.find(stmt) != following_set.end();
}

bool FollowStore::IsFollowerStar(std::string stmt) {
  return follower_star_set.find(stmt) != follower_star_set.end();
}

bool FollowStore::IsFollowingStar(std::string stmt) {
  return following_star_set.find(stmt) != following_star_set.end();
}

void FollowStore::Init(int num_stmts) {
  node n = {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()};

  for (int i = 1; i <= num_stmts; i++) {
    rs_map[std::to_string(i)] = n;
  }
}

void FollowStore::AddFollow(std::string follower, std::string following) {
  all_follow_pairs.emplace(std::pair<std::string, std::string>(follower, following));

  if (rs_map.find(follower) == rs_map.end()) {
    rs_map.insert({follower, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (rs_map.find(following) == rs_map.end()) {
    rs_map.insert({following, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  rs_map.at(follower).following = following;
  rs_map.at(following).follower = follower;
  follower_set.insert(follower);
  following_set.insert(following);
}

void FollowStore::AddFollowStar(std::string follower, std::string following) {
  all_follow_star_pairs.emplace(std::pair<std::string, std::string>(follower, following));

  if (rs_map.find(follower) == rs_map.end()) {
    rs_map.insert({follower, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (rs_map.find(following) == rs_map.end()) {
    rs_map.insert({following, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  rs_map.at(follower).following_star.insert(following);
  rs_map.at(following).follower_star.insert(follower);
  follower_star_set.insert(follower);
  following_star_set.insert(following);
}

// Used for follower(s1, s2)
bool FollowStore::FollowExists(std::pair<std::string, std::string> pair) {
  return all_follow_pairs.find(pair) != all_follow_pairs.end();
}

// Used for follower*(s1, s2)
bool FollowStore::FollowStarExists(std::pair<std::string, std::string> pair) {
  return all_follow_star_pairs.find(pair) != all_follow_star_pairs.end();
}

std::unordered_set<std::string> FollowStore::GetAllFollowers() {
  return follower_set;
}

std::string FollowStore::GetFollowerOf(std::string stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).follower;
  }
  return "0";
}

std::string FollowStore::GetFollowingOf(std::string stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).following;
  }
  return "0";
}

std::unordered_set<std::string> FollowStore::GetFollowerStarOf(std::string stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).follower_star;
  }
  return {};
}

std::unordered_set<std::string> FollowStore::GetFollowingStarOf(std::string stmt) {
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
