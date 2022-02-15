#ifndef PKB_H
#define PKB_H

#include <string>
#include <unordered_set>
#include <vector>

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

  void AddFollowStmt(std::string stmt1, std::string stmt2);
  void AddFollowStarStmt(std::string stmt1, std::string stmt2);

  void AddParentStmt(std::string stmt1, std::string stmt2);
  void AddParentStarStmt();

  void AddPattern(std::string lhs, std::string rhs);

  /* Getters */
  std::unordered_set<std::string> GetStmt(StmtType type);

  std::unordered_set<std::string> GetVarUsedByStmt(std::string stmt);
  std::unordered_set<std::string> GetStmtUsedByVar(std::string var);
  std::unordered_set<std::string> GetVarUsedByProc(std::string proc);
  std::unordered_set<std::string> GetProcUsedByVar(std::string var);
  std::unordered_set<std::string> GetAllStmtUsing();
  std::unordered_set<std::string> GetAllProcUsing();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsageStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllUsageProcVar();

  std::unordered_set<std::string> GetVarModByStmt(std::string stmt);
  std::unordered_set<std::string> GetStmtModByVar(std::string var);
  std::unordered_set<std::string> GetVarModByProc(std::string proc);
  std::unordered_set<std::string> GetProcModByVar(std::string var);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllModProcVar();

  std::unordered_set<std::string> GetFollowerOf(std::string stmt);
  std::unordered_set<std::string> GetFollowingOf(std::string stmt);

  int GetPattern(std::string lhs, std::string rhs);

  /* Checkers */
  bool IsUsageStmtVarExist(std::pair<std::string, std::string> pair);
  bool IsUsageProcVarExist(std::pair<std::string, std::string> pair);

  bool IsModifyStmtVarExist(std::pair<std::string, std::string> pair);
  bool IsModifyProcVarExist(std::pair<std::string, std::string> pair);

  bool IsFollowExist();
  bool IsFollowStarExist();

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

  std::unordered_map<std::string, std::string> pattern_map;

  FollowStore follow_store;
  ParentStore parent_store;
  UsageStore usage_store;
  ModifyStore modify_store;
};

#endif //PKB_H
