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
    std::string follower;
    std::string following;
    std::unordered_set<std::string> follower_star;
    std::unordered_set<std::string> following_star;
  };

  bool IsFollower(int stmt);

  bool IsFollowing(int stmt);

  bool IsFollowerStar(int stmt);

  bool IsFollowingStar(int stmt);

  bool FollowExists(std::pair<std::string, std::string> pair);

  bool FollowStarExists(std::pair<std::string, std::string> pair);

  void Init(int num_stmts);

  void AddFollow(std::string parent, std::string child);

  std::string GetFollowerOf(std::string stmt);

  std::string GetFollowingOf(std::string stmt);

  std::unordered_set<std::string> GetFollowerStarOf(std::string stmt);

  std::unordered_set<std::string> GetFollowingStarOf(std::string stmt);

  //std::unordered_set<std::pair<int, int>, pair_hash> get_follow_pairs();

 private:
  std::unordered_map<std::string, node> rs_map;
  std::unordered_set<int> follower_star_set;
  std::unordered_set<int> following_star_set;
  std::unordered_set<int> follower_set;
  std::unordered_set<int> following_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash > all_follow_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash > all_follow_star_pairs;
};

#endif //FOLLOW_STORE_H
