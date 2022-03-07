#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include "store.h"

// A struct to maintain every FollowNode's relationships
struct FollowNode {
  std::string follower;
  std::string following;
  std::unordered_set<std::string> follower_star;
  std::unordered_set<std::string> following_star;
};

// A store class that maintains all Parent APIs and relationships
class FollowStore : public Store {
 private:
  std::unordered_map<std::string, FollowNode> rs_map;
  std::unordered_set<std::string> follower_set;
  std::unordered_set<std::string> following_set;
  std::unordered_set<std::string> follower_star_set;
  std::unordered_set<std::string> following_star_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_follow_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_follow_star_pairs;

 public:
  explicit FollowStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddFollow(std::string const &parent, std::string const &child);
  void AddFollowStar(std::string const &parent, std::string const &child);
  void AddFollowHelper(bool is_star, std::string const &follower, std::string const &following);
  [[nodiscard]] bool IsFollower(std::string const &stmt);
  [[nodiscard]] bool IsFollowing(std::string const &stmt);
  [[nodiscard]] bool IsFollowerStar(std::string const &stmt);
  [[nodiscard]] bool IsFollowingStar(std::string const &stmt);
  [[nodiscard]] bool FollowExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool FollowStarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetFollowerOf(std::string const &stmt);
  [[nodiscard]] std::string GetFollowingOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetFollowerStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetFollowingStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type1,
                                                                                                        StmtType type2);
};

#endif //FOLLOW_STORE_H
