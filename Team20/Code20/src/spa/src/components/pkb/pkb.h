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
  PROC,
  STMT,
  WHILE,
  READ,
  PRINT,
  CALL,
  IF,
  ASSIGN,
  VARS,
  CONSTS
};

class PKB {
 private:
  FollowStore follow_store;
  ParentStore parent_store;
  UsageStore usage_store;
  ModifyStore modify_store;

  std::unordered_set<std::string> stmt_list;
  std::unordered_set<std::string> while_stmt_list;
  std::unordered_set<std::string> read_stmt_list;
  std::unordered_set<std::string> print_stmt_list;
  std::unordered_set<std::string> call_stmt_list;
  std::unordered_set<std::string> if_stmt_list; //need to create separate else-then or treat this as a block?
  std::unordered_set<std::string> assign_stmt_list;
  std::unordered_set<std::string> proc_list;
  std::unordered_set<std::string> var_list;
  std::unordered_set<std::string> const_list;

  std::unordered_map<std::string, std::pair<std::string, std::string>> pattern_map;

 public:
  PKB();
  FollowStore &GetFollowStore();
  ModifyStore &GetModifyStore();
  ParentStore &GetParentStore();
  UsageStore &GetUsageStore();

  /* Adders */
  void AddStmt(std::string name, StmtType type);
  void AddPattern(std::string stmt, std::string lhs, std::string rhs);

  /* Getters */
  std::unordered_set<std::string> GetStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsesStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type1, StmtType type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type1,
                                                                                          StmtType type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type1, StmtType type2);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type1,
                                                                                          StmtType type2);

  std::unordered_set<std::string> GetStmtWithPattern(std::string lhs, std::string rhs);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonym(std::string rhs);
};

#endif //PKB_H