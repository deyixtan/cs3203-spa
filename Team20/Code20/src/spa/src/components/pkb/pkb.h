#ifndef PKB_H
#define PKB_H

#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "usage_store.h"
#include "modify_store.h"
#include "follow_store.h"
#include "parent_store.h"

//typedef short PROC;

//class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

enum StmtType {
  NONE,
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
 public:
  //VarTable* varTable;
  //int setProcToAST(PROC p, TNode* r);
  //TNode* getRootAST (PROC p);
  static PKB *GetInstance();

  /* Adders */
  void AddStmt(std::string name, StmtType type);

  void AddUsageStmtVar(std::string stmt, std::string variable);
  void AddUsageProcVar(std::string proc, std::string var);

  void AddModifyStmtVar(std::string stmt, std::string variable);
  void AddModifyProcVar(std::string proc, std::string var);

  void AddPattern(std::string stmt, std::string lhs, std::string rhs);
  void AddFollowStmt(std::string stmt1, std::string stmt2);
  void AddFollowStarStmt(std::string stmt1, std::string stmt2);

  void AddParentStmt(std::string stmt1, std::string stmt2);
  void AddParentStarStmt(std::string stmt, std::vector<std::string> visited);

  /* Getters */
  std::unordered_set<std::string> GetStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsesStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllFollowStarStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type);

  std::unordered_set<std::string> GetVarUsedByStmt(std::string stmt);
  std::unordered_set<std::string> GetStmtUsedByVar(std::string var);
  std::unordered_set<std::string> GetVarUsedByProc(std::string proc);
  std::unordered_set<std::string> GetProcUsedByVar(std::string var);
  std::unordered_set<std::string> GetAllStmtUsing();
  std::unordered_set<std::string> GetAllProcUsing();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsageStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsageProcVar();
  std::unordered_set<std::string> GetAllStmtModify();
  std::unordered_set<std::string> GetAllProcModify();

  std::unordered_set<std::string> GetVarModByStmt(std::string stmt);
  std::unordered_set<std::string> GetStmtModByVar(std::string var);
  std::unordered_set<std::string> GetVarModByProc(std::string proc);
  std::unordered_set<std::string> GetProcModByVar(std::string var);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModProcVar();

  std::unordered_set<std::string> GetStmtWithPattern(std::string lhs, std::string rhs);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonym(std::string rhs);
  std::string GetFollowOf(std::string stmt);
  std::unordered_set<std::string> GetFollowStarOf(std::string stmt);
  std::string GetParentOf(std::string stmt);
  std::unordered_set<std::string> GetParentStarOf(std::string stmt);

  /* Checkers */
  bool IsUsageStmtVarExist(std::pair<std::string, std::string> pair);
  bool IsUsageProcVarExist(std::pair<std::string, std::string> pair);

  bool IsModifyStmtVarExist(std::pair<std::string, std::string> pair);
  bool IsModifyProcVarExist(std::pair<std::string, std::string> pair);

  bool IsFollowExist(std::pair<std::string, std::string> pair);
  bool IsFollowStarExist(std::pair<std::string, std::string> pair);

  bool IsFollower(std::string stmt);
  bool IsFollowing(std::string stmt);
  bool IsFollowerStar(std::string stmt);
  bool IsFollowingStar(std::string stmt);

  bool IsParent(std::string stmt);
  bool IsChild(std::string stmt);
  bool IsAnce(std::string stmt);
  bool IsDesc(std::string stmt);
  bool ParentChildExists(std::string stmt1, std::string stmt2);
  bool AnceExists(std::string curr, std::string ance);
  bool DescExists(std::string curr, std::string desc);

 private:
  PKB();
  static PKB *instance;

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

  std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> pattern_map;

  FollowStore follow_store;
  ParentStore parent_store;
  UsageStore usage_store;
  ModifyStore modify_store;
};

#endif //PKB_H
