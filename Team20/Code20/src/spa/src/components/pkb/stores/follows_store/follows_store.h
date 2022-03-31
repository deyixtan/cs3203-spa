#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

// A store class that maintains all Parent APIs and relationships
class FollowsStore : public StmtStmtStore {
 private:

 public:
  explicit FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                        std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddFollow(std::string const &follower, std::string const &following);
  void AddFollowStar(std::string const &follower, std::string const &following);
  [[nodiscard]] bool IsFollowValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsFollowStarValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetFollowerOf(std::string const &stmt);
  [[nodiscard]] std::string GetFollowingOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetFollowerStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetFollowingStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type1,
                                                                                                        StmtType type2);
};

#endif //FOLLOW_STORE_H
