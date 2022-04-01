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

// A struct to maintain every FollowsNode's relationships
struct NextNode {
  std::unordered_set<std::string> before;
  std::unordered_set<std::string> next;
  std::unordered_set<std::string> before_star_set;
  std::unordered_set<std::string> next_star_set;
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
  std::unordered_map<StmtType,
                     std::unordered_map<StmtType, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>>
      type_pair_map;
  std::unordered_map<StmtType,
                     std::unordered_map<StmtType, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>>
      star_type_pair_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_star_pairs;

 protected:
  std::unordered_map<std::string, FollowsNode> follows_rs_map;
  std::unordered_map<std::string, ParentChildNode> parent_rs_map;
  std::unordered_map<std::string, CallsNode> calls_rs_map;
  std::unordered_map<std::string, NextNode> next_rs_map;

 public:
  explicit StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddUpperLower(StoreType store_type,
                     std::string const &upper,
                     std::string const &lower);
  void AddUpperLowerStar(StoreType store_type,
                         std::string const &upper,
                         std::string const &lower,
                         std::vector<std::string> const &visited);
  void AddFollows(bool is_star, std::string const &upper, std::string const &lower);
  void AddParent(bool is_star,
                 std::string const &upper,
                 std::string const &lower,
                 std::vector<std::string> const &visited);
  void AddCalls(bool is_star, std::string const &upper, std::string const &lower);
  void AddNext(bool is_star, std::string const &upper, std::string const &lower);
  void WipeNextStar();
  [[nodiscard]] bool IsValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsStarValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetUpperOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetUpperSetOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetLowerOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::string GetLowerOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetUpperStarOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetLowerStarOf(StoreType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetPairByType(StmtType type1,
                                                                                                 StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStarPairByType(StmtType type1,
                                                                                                     StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStarPairs();
  [[nodiscard]] void GetLowerStarOfHelper(std::string const &stmt,
                                          std::unordered_set<std::string> &res,
                                          std::unordered_set<std::string> &visited);
  [[nodiscard]] void GetUpperStarOfHelper(std::string const &stmt,
                                          std::unordered_set<std::string> &res,
                                          std::unordered_set<std::string> &visited);
};

#endif //FOLLOWS_PARENT_STORE_H
