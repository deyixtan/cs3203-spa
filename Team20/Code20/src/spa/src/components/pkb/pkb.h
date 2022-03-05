#ifndef PKB_H
#define PKB_H

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "stores/usage_store.h"
#include "stores/modify_store.h"
#include "stores/follow_store.h"
#include "stores/parent_store.h"

enum StmtType {
  STMT,
  READ,
  PRINT,
  WHILE,
  IF,
  ASSIGN,
  VARS,
  CONSTS,
  PROC,
  CALL,
  COUNT = CALL // get max value of StmtType
};

class PKB {
 private:
  FollowStore follow_store;
  ParentStore parent_store;
  UsageStore usage_store;
  ModifyStore modify_store;

  std::vector<std::unordered_set<std::string>> stmt_vector;
  std::unordered_map<std::string, std::pair<std::string, std::string>> pattern_map;

 public:
  PKB();
  [[nodiscard]] FollowStore &GetFollowStore();
  [[nodiscard]] ModifyStore &GetModifyStore();
  [[nodiscard]] ParentStore &GetParentStore();
  [[nodiscard]] UsageStore &GetUsageStore();
  void InitStatementVector();

  void AddStmt(std::string name, StmtType type);
  void AddPattern(std::string stmt, std::string lhs, std::string rhs);
  [[nodiscard]] std::unordered_set<std::string> GetStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsesStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type1, StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type1,
                                                                                          StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type1, StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type1,
                                                                                          StmtType type2);

  [[nodiscard]] std::unordered_set<std::string> GetStmtWithPattern(std::string lhs, std::string rhs);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonym(std::string rhs);
};

#endif //PKB_H
