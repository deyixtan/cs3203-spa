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

  bool IsFollower(int stmt);

  bool IsFollowing(int stmt);

  bool IsFollowerStar(int stmt);

  bool IsFollowingStar(int stmt);

  bool FollowExists(std::pair<int, int> pair);

  bool FollowStarExists(std::pair<int, int> pair);

  void Init(int num_stmts);

  void AddFollow(int parent, int child);

  int GetFollowerOf(int stmt);

  int GetFollowingOf(int stmt);

  std::unordered_set<int> GetFollowerStarOf(int stmt);

  std::unordered_set<int> GetFollowingStarOf(int stmt);

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
