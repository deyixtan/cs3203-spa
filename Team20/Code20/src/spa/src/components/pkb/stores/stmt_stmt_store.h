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

// A struct to maintain every ParentNode's relationships
struct ParentChildNode {
  std::string parent;
  std::unordered_set<std::string> child;
  std::unordered_set<std::string> ance;
  std::unordered_set<std::string> desc;
};

// A struct to maintain every CallsNode's relationships
struct CallsNode {
  std::unordered_set<std::string> callers_set;
  std::unordered_set<std::string> callees_set;
  std::unordered_set<std::string> callers_star_set;
  std::unordered_set<std::string> callees_star_set;
};

class StmtStmtStore : public Store {
 private:
  std::unordered_map<std::string, FollowsNode> follows_rs_map;
  std::unordered_map<std::string, ParentChildNode> parent_rs_map;
  std::unordered_map<std::string, CallsNode> calls_rs_map;
  std::unordered_set<std::string> upper_set;
  std::unordered_set<std::string> lower_set;
  std::unordered_set<std::string> upper_star_set;
  std::unordered_set<std::string> lower_star_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_star_pairs;

 public:
  explicit StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddUpperLower(StoreType type, std::string const &upper, std::string const &lower);
  void AddUpperLowerStar(StoreType type, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited);
  void AddFollows(bool is_star, std::string const &upper, std::string const &lower);
  void AddParent(bool is_star, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited);
  void AddCalls(bool is_star, std::string const &upper, std::string const &lower);
  [[nodiscard]] bool IsUpper(std::string const &stmt);
  [[nodiscard]] bool IsLower(std::string const &stmt);
  [[nodiscard]] bool IsUpperStar(std::string const &stmt);
  [[nodiscard]] bool IsLowerStar(std::string const &stmt);
  [[nodiscard]] bool IsValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsStarValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetUpperOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetUpperOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetLowerOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::string GetLowerOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetUpperStarOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetLowerStarOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStarPairs();
};

#endif //FOLLOWS_PARENT_STORE_H
