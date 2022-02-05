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
  all_follow_pairs.emplace(std::pair<int, int>(follower, following));
  //need to handle follow_star_pairs
}

// Used for follower(s1, s2)
bool FollowStore::follow_exists(std::pair<int, int> pair) {
  return all_follow_pairs.find(pair) != all_follow_pairs.end();
}

// Used for follower*(s1, s2)
bool FollowStore::follow_star_exists(std::pair<int, int> pair) {
  return all_follow_star_pairs.find(pair) != all_follow_star_pairs.end();
}

int FollowStore::get_follower_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    return rs_map.at(stmt).follower;
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
