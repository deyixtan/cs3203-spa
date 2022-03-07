#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include "store.h"

// A store class that maintains all Parent APIs and relationships
class FollowStore : public Store {
 public:
  explicit FollowStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);

  // A struct to maintain every node's relationships
  struct node {
    std::string follower;
    std::string following;
    std::unordered_set<std::string> follower_star;
    std::unordered_set<std::string> following_star;
  };

  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type1,
                                                                                                        StmtType type2);

  bool IsFollower(std::string const &stmt);

  bool IsFollowing(std::string const &stmt);

  bool IsFollowerStar(std::string const &stmt);

  bool IsFollowingStar(std::string const &stmt);

  bool FollowExists(std::pair<std::string, std::string> const &pair);

  bool FollowStarExists(std::pair<std::string, std::string> const &pair);

  void Init(int num_stmts);

  void AddFollow(std::string const &parent, std::string const &child);

  void AddFollowStar(std::string const &parent, std::string const &child);

  std::unordered_set<std::string> GetAllFollowers();

  std::string GetFollowerOf(std::string const &stmt);

  std::string GetFollowingOf(std::string const &stmt);

  std::unordered_set<std::string> GetFollowerStarOf(std::string const &stmt);

  std::unordered_set<std::string> GetFollowingStarOf(std::string const &stmt);

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowPairs();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowStarPairs();

 private:
  std::unordered_map<std::string, node> rs_map;
  std::unordered_set<std::string> follower_star_set;
  std::unordered_set<std::string> following_star_set;
  std::unordered_set<std::string> follower_set;
  std::unordered_set<std::string> following_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_follow_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_follow_star_pairs;
};

#endif //FOLLOW_STORE_H
