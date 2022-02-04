#include "follow_store.h"

FollowStore::FollowStore() {}

bool FollowStore::is_follower(int stmt) {
  return follower_set.find(stmt) != follower_set.end();
}

bool FollowStore::is_following(int stmt) {
  return following_set.find(stmt) != following_set.end();
}

bool FollowStore::is_follower_star(int stmt) {
  return follower_star_set.find(stmt) != follower_star_set.end();
}

bool FollowStore::is_following_star(int stmt) {
  return following_star_set.find(stmt) != following_star_set.end();
}

void FollowStore::rs_init(int num_stmts) {
  node n = {0, 0, std::unordered_set<int>(), std::unordered_set<int>()};

  for (int i = 1; i <= num_stmts; i++) {
    rs_map[i] = n;
  }
}

void FollowStore::add_follow(int follower, int following) {
  if (!follow_exists(follower, following)) {
    follow_set.insert(std::make_pair(follower, following));
    rs_map.at(follower).following = following;
    rs_map.at(following).follower = follower;
  }

  rs_map.at(follower).following_star.insert(following);
  rs_map.at(following).follower_star.insert(follower);
}

// Used for follower(s1, s2)
bool FollowStore::follow_exists(int stmt1, int stmt2) {
  std::pair<int, int> p = std::make_pair(stmt1, stmt2);
  return follow_set.find(p) != follow_set.end();
}

// Used for follower*(s1, s2)
bool FollowStore::follower_star_exists(int curr, int follower_star) {
  std::unordered_set<int> all_follower_star = get_follower_star_of(curr);
  if (all_follower_star.find(follower_star) != all_follower_star.end()) {
    return true;
  }
  return false;
}

// Used for follower*(s1, s2)
bool FollowStore::following_star_exists(int curr, int following_star) {
  std::unordered_set<int> all_following_star = get_following_star_of(curr);
  if (all_following_star.find(following_star) != all_following_star.end()) {
    return true;
  }
  return false;
}

int FollowStore::get_follower_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    node n = rs_map.at(stmt);
    return n.follower;
  }
  return 0;
}

int FollowStore::get_following_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    node n = rs_map.at(stmt);
    return n.following;
  }
  return 0;
}

std::unordered_set<int> FollowStore::get_follower_star_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    node n = rs_map.at(stmt);
    return n.follower_star;
  }
  return {};
}

std::unordered_set<int> FollowStore::get_following_star_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    node n = rs_map.at(stmt);
    return n.following_star;
  }
  return {};
}

std::unordered_set<std::pair<int, int>, pair_hash> FollowStore::get_follow_pairs() {
  return follow_set;
}
