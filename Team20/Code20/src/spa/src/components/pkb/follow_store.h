#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "hash.h"

// A store class that maintains all Parent APIs and relationships
class FollowStore {
 public:
  FollowStore();

  // A struct to maintain every node's relationships
  struct node {
    int follower;
    int following;
    std::unordered_set<int> follower_star;
    std::unordered_set<int> following_star;
  };

  bool is_follower(int stmt);

  bool is_following(int stmt);

  bool is_follower_star(int stmt);

  bool is_following_star(int stmt);

  bool follow_exists(std::pair<int, int> pair);

  bool follow_star_exists(std::pair<int, int> pair);

  void rs_init(int num_stmts);

  void add_follow(int parent, int child);

  int get_follower_of(int stmt);

  int get_following_of(int stmt);

  std::unordered_set<int> get_follower_star_of(int stmt);

  std::unordered_set<int> get_following_star_of(int stmt);

  //std::unordered_set<std::pair<int, int>, pair_hash> get_follow_pairs();

 private:
  std::unordered_map<int, node> rs_map;
  std::unordered_set<int> follower_star_set;
  std::unordered_set<int> following_star_set;
  std::unordered_set<int> follower_set;
  std::unordered_set<int> following_set;
  std::unordered_set<std::pair<int, int>, pair_hash > all_follow_pairs;
  std::unordered_set<std::pair<int, int>, pair_hash > all_follow_star_pairs;
};

#endif //FOLLOW_STORE_H
