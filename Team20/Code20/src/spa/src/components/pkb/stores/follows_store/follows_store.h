#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

// A store class that maintains all Parent APIs and relationships
class FollowsStore : public StmtStmtStore {
 private:

 public:
  explicit FollowsStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                        std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddFollow(IDENT const &follower, IDENT const &following);
  void AddFollowStar(IDENT const &follower, IDENT const &following);
  [[nodiscard]] bool IsFollowsPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsFollowsStarPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT GetFollowerOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT GetFollowingOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetFollowerStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetFollowingStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllFollowStmt(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllFollowStarStmt(StmtType type1, StmtType type2);
};

#endif //FOLLOW_STORE_H
