#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "UsageStore.h"
#include "ModifyStore.h"

//typedef short PROC;

//class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

struct hashFunction {
  size_t operator()(const std::pair<int, int> &x) const {
    return x.first ^ x.second;
  }
};

enum stmtType {
  NONE,
  PROC,
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
  //static VarTable* varTable;
  //static int setProcToAST(PROC p, TNode* r);
  //static TNode* getRootAST (PROC p);
  PKB();

  /* Adders */
  void add_stmt(int stmt = 0, std::string name = "", stmtType type = NONE);

  bool add_usage_stmt_var(int stmt, std::string variable);
  bool add_usage_proc_var(std::string proc, std::string var);

  bool add_modify_stmt_var(int stmt, std::string variable);
  bool add_modify_proc_var(std::string proc, std::string var);

  /* Getters */

  template<typename T>
  T get_stmt(stmtType type);

  std::unordered_set<std::string> get_var_used_by_stmt(int stmt);
  std::unordered_set<int> get_stmt_used_by_var(std::string var);
  std::unordered_set<std::string> get_var_used_by_proc(std::string proc);
  std::unordered_set<std::string> get_proc_used_by_var(std::string var);
  std::unordered_set<std::pair<int, std::string>, hashFunction> get_all_usage_stmt_var();
  std::unordered_set<std::pair<std::string, std::string>, hashFunction> get_all_usage_proc_var();

  std::unordered_set<std::string> get_var_mod_by_stmt(int stmt);
  std::unordered_set<int> get_stmt_mod_by_var(std::string var);
  std::unordered_set<std::string> get_var_mod_by_proc(std::string proc);
  std::unordered_set<std::string> get_proc_mod_by_var(std::string var);
  std::unordered_set<std::pair<int, std::string>, hashFunction> get_all_mod_stmt_var();
  std::unordered_set<std::pair<std::string, std::string>, hashFunction> get_all_mod_proc_var();

  /* Checkers */
  bool usage_stmt_var_exists(std::pair<int, std::string> pair);
  bool usage_proc_var_exists(std::pair<std::string, std::string> pair);

  bool modify_stmt_var_exists(std::pair<int, std::string> pair);
  bool modify_proc_var_exists(std::pair<std::string, std::string> pair);

 private:
  static std::unordered_set<std::string> proc_list;
  static std::unordered_set<int> while_stmt_list;
  static std::unordered_set<int> read_stmt_list;
  static std::unordered_set<int> print_stmt_list;
  static std::unordered_set<int> call_stmt_list;
  static std::unordered_set<int> if_stmt_list; //need to create separate else-then or treat this as a block?
  static std::unordered_set<int> assign_stmt_list;

  static std::unordered_set<std::string> var_list;
  static std::unordered_set<std::string> const_list;

  //static FollowStore fStore;
  //static ParentStore pStore;
  static UsageStore usage_store;
  static ModifyStore modify_store;
};