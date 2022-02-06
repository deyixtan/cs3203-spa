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
  void add_stmt(int stmt = 0, std::string name = "", StmtType type = NONE);

  void add_usage_stmt_var(int stmt, std::string variable);
  void add_usage_proc_var(std::string proc, std::string var);

  void add_modify_stmt_var(int stmt, std::string variable);
  void add_modify_proc_var(std::string proc, std::string var);

  /* Getters */

  template<typename T>
  T get_stmt(StmtType type);

  std::unordered_set<int> temp_get_stmt(); //to be removed
  std::unordered_set<std::string> temp_get_vars(); //to be removed

  std::unordered_set<std::string> get_var_used_by_stmt(int stmt);
  std::unordered_set<int> get_stmt_used_by_var(std::string var);
  std::unordered_set<std::string> get_var_used_by_proc(std::string proc);
  std::unordered_set<std::string> get_proc_used_by_var(std::string var);
  std::unordered_set<std::pair<int, std::string>, pair_hash> get_all_usage_stmt_var();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_usage_proc_var();

  std::unordered_set<std::string> get_var_mod_by_stmt(int stmt);
  std::unordered_set<int> get_stmt_mod_by_var(std::string var);
  std::unordered_set<std::string> get_var_mod_by_proc(std::string proc);
  std::unordered_set<std::string> get_proc_mod_by_var(std::string var);
  std::unordered_set<std::pair<int, std::string>, pair_hash> get_all_mod_stmt_var();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_mod_proc_var();

  /* Checkers */
  bool usage_stmt_var_exists(std::pair<int, std::string> pair);
  bool usage_proc_var_exists(std::pair<std::string, std::string> pair);

  bool modify_stmt_var_exists(std::pair<int, std::string> pair);
  bool modify_proc_var_exists(std::pair<std::string, std::string> pair);

 private:
  PKB();
  static PKB *instance;

  std::unordered_set<std::string> proc_list;
  std::unordered_set<int> stmt_list;
  std::unordered_set<int> while_stmt_list;
  std::unordered_set<int> read_stmt_list;
  std::unordered_set<int> print_stmt_list;
  std::unordered_set<int> call_stmt_list;
  std::unordered_set<int> if_stmt_list; //need to create separate else-then or treat this as a block?
  std::unordered_set<int> assign_stmt_list;

  std::unordered_set<std::string> var_list;
  std::unordered_set<std::string> const_list;

  FollowStore follow_store;
  ParentStore parent_Store;
  UsageStore usage_store;
  ModifyStore modify_store;
};

#endif //PKB_H
