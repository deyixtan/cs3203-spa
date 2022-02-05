#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "hash.h"

// A struct to maintain every node's relationships
struct node {
  int follower;
  int following;
  std::unordered_set<int> follower_star;
  std::unordered_set<int> following_star;
};

// A store class that maintains all Parent APIs and relationships
class FollowStore {
 public:
  FollowStore();

  bool is_follower(int stmt);

  bool is_following(int stmt);

  bool is_follower_star(int stmt);

  bool is_following_star(int stmt);

  bool follow_exists(int stmt1, int stmt2);

  bool follower_star_exists(int curr, int ance);

  bool following_star_exists(int curr, int desc);

  void rs_init(int num_stmts);

  void add_follow(int parent, int child);

  int get_follower_of(int stmt);

  int get_following_of(int stmt);

  std::unordered_set<int> get_follower_star_of(int stmt);

  std::unordered_set<int> get_following_star_of(int stmt);

  std::unordered_set<std::pair<int, int>, pair_hash> get_follow_pairs();

 private:
  std::unordered_map<int, node> rs_map;
  std::unordered_set<std::pair<int, int>, pair_hash> follow_set;
  std::unordered_set<int> follower_star_set;
  std::unordered_set<int> following_star_set;
  std::unordered_set<int> follower_set;
  std::unordered_set<int> following_set;
};

#endif //FOLLOW_STORE_H
