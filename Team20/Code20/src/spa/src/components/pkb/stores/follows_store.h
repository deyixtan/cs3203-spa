#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include "follows_parent_store.h"

// A store class that maintains all Parent APIs and relationships
class FollowsStore : public FollowsParentStore {
 private:

 public:
  explicit FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddFollow(std::string const &parent, std::string const &child);
  void AddFollowStar(std::string const &parent, std::string const &child);
  [[nodiscard]] bool IsFollower(std::string const &stmt);
  [[nodiscard]] bool IsFollowing(std::string const &stmt);
  [[nodiscard]] bool IsFollowerStar(std::string const &stmt);
  [[nodiscard]] bool IsFollowingStar(std::string const &stmt);
  [[nodiscard]] bool IsFollowExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsFollowStarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetFollowerOf(std::string const &stmt);
  [[nodiscard]] std::string GetBaseFollowingOf(std::string const &stmt);
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
