#ifndef FOLLOWS_PARENT_STORE_H
#define FOLLOWS_PARENT_STORE_H

#include "store.h"

// A struct to maintain every FollowsNode's relationships
struct FollowsNode {
  std::string follower;
  std::string following;
  std::unordered_set<std::string> follower_star;
  std::unordered_set<std::string> following_star;
};

// A struct to maintain every node's relationships
struct ParentChildNode {
  std::string parent;
  std::unordered_set<std::string> child;
  std::unordered_set<std::string> ance;
  std::unordered_set<std::string> desc;
};

class FollowsParentStore : public Store {
 private:
  std::unordered_map<std::string, FollowsNode> follows_rs_map;
  std::unordered_map<std::string, ParentChildNode> parent_rs_map;
  std::unordered_set<std::string> upper_set;
  std::unordered_set<std::string> lower_set;
  std::unordered_set<std::string> upper_star_set;
  std::unordered_set<std::string> lower_star_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_star_pairs;

 public:
  explicit FollowsParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddUpperLower(bool isFollows, std::string const &upper, std::string const &lower);
  void AddUpperLowerStar(bool isFollows, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited);
  void AddFollowsUpperLower(bool is_star, std::string const &upper, std::string const &lower);
  void AddParentUpperLower(bool is_star, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited);
  [[nodiscard]] bool IsUpper(std::string const &stmt);
  [[nodiscard]] bool IsLower(std::string const &stmt);
  [[nodiscard]] bool IsUpperStar(std::string const &stmt);
  [[nodiscard]] bool IsLowerStar(std::string const &stmt);
  [[nodiscard]] bool IsExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsStarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetUpperOf(bool isFollows, std::string const &stmt);
  [[nodiscard]] std::string GetFollowingOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetChildOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetUpperStarOf(bool isFollows, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetLowerStarOf(bool isFollows, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStarPairs();
};

#endif //FOLLOWS_PARENT_STORE_H
