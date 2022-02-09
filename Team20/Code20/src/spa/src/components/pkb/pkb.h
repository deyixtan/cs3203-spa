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
  static PKB *get_instance();

  /* Adders */
  void add_stmt(std::string name, StmtType type);

  void add_usage_stmt_var(std::string stmt, std::string variable);
  void add_usage_proc_var(std::string proc, std::string var);

  void add_modify_stmt_var(std::string stmt, std::string variable);
  void add_modify_proc_var(std::string proc, std::string var);

  /* Getters */
  std::unordered_set<std::string> get_stmt(StmtType type);

  std::unordered_set<std::string> get_var_used_by_stmt(std::string stmt);
  std::unordered_set<std::string> get_stmt_used_by_var(std::string var);
  std::unordered_set<std::string> get_var_used_by_proc(std::string proc);
  std::unordered_set<std::string> get_proc_used_by_var(std::string var);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_usage_stmt_var();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_usage_proc_var();

  std::unordered_set<std::string> get_var_mod_by_stmt(std::string stmt);
  std::unordered_set<std::string> get_stmt_mod_by_var(std::string var);
  std::unordered_set<std::string> get_var_mod_by_proc(std::string proc);
  std::unordered_set<std::string> get_proc_mod_by_var(std::string var);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_mod_stmt_var();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_mod_proc_var();

  /* Checkers */
  bool is_usage_stmt_var_exist(std::pair<std::string, std::string> pair);
  bool is_usage_proc_var_exist(std::pair<std::string, std::string> pair);

  bool is_modify_stmt_var_exist(std::pair<std::string, std::string> pair);
  bool is_modify_proc_var_exist(std::pair<std::string, std::string> pair);

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

  FollowStore follow_store;
  ParentStore parent_Store;
  UsageStore usage_store;
  ModifyStore modify_store;
};

#endif //PKB_H
