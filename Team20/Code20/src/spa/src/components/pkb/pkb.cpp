#include <vector>

#include "pkb.h"
//#include "components/source_subsystem/TNode.h"

/*
int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}
*/

PKB* PKB::instance = NULL;

PKB::PKB() {}

PKB* PKB::get_instance() {
  if (!instance) {
    instance = new PKB;
  }
  return instance;
}

/* Adders */
void PKB::add_stmt(std::string stmt, StmtType type) {
  switch (type) {
    case STMT:stmt_list.insert(stmt);
      break;
    case WHILE:while_stmt_list.insert(stmt);
      break;
    case READ:read_stmt_list.insert(stmt);
      break;
    case PRINT:print_stmt_list.insert(stmt);
      break;
    case CALL:call_stmt_list.insert(stmt);
      break;
    case IF:if_stmt_list.insert(stmt);
      break;
    case ASSIGN:assign_stmt_list.insert(stmt);
      break;
    case PROC:proc_list.insert(stmt);
      break;
    case VARS:var_list.insert(stmt);
      break;
    case CONSTS:const_list.insert(stmt);
       break;
    default:break;
  }
}

void PKB::add_usage_stmt_var(std::string stmt, std::string var) {
  usage_store.add_stmt_var(stmt, var);
}

void PKB::add_usage_proc_var(std::string proc, std::string var) {
  usage_store.add_proc_var(proc, var);
}

void PKB::add_modify_stmt_var(std::string stmt, std::string var) {
  modify_store.add_stmt_var(stmt, var);
}

void PKB::add_modify_proc_var(std::string proc, std::string var) {
  modify_store.add_proc_var(proc, var);
}

/* Getters */

std::unordered_set<std::string> PKB::get_stmt(StmtType type) {
  switch (type) {
    case STMT: return stmt_list;
    case WHILE:return while_stmt_list;
    case READ:return read_stmt_list;
    case PRINT:return print_stmt_list;
    case CALL:return call_stmt_list;
    case IF:return if_stmt_list;
    case ASSIGN:return assign_stmt_list;
    case PROC:return proc_list;
    case VARS:return var_list;
    case CONSTS:return const_list;
    default:break;
  }
}

std::unordered_set<std::string> PKB::get_var_used_by_stmt(std::string stmt) {
  return usage_store.get_var_used_by_stmt(stmt);
}

std::unordered_set<std::string> PKB::get_stmt_used_by_var(std::string var) {
  return usage_store.get_stmt_used_by_var(var);
}

std::unordered_set<std::string> PKB::get_var_used_by_proc(std::string proc) {
  return usage_store.get_var_used_by_proc(proc);
}

std::unordered_set<std::string> PKB::get_proc_used_by_var(std::string var) {
  return usage_store.get_proc_used_by_var(var);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::get_all_usage_stmt_var() {
  return usage_store.get_all_stmt_var();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::get_all_usage_proc_var() {
  return usage_store.get_all_proc_var();
}

std::unordered_set<std::string> PKB::get_var_mod_by_stmt(std::string stmt) {
  return modify_store.get_var_mod_by_stmt(stmt);
}

std::unordered_set<std::string> PKB::get_stmt_mod_by_var(std::string var) {
  return modify_store.get_stmt_mod_by_var(var);
}

std::unordered_set<std::string> PKB::get_var_mod_by_proc(std::string proc) {
  return modify_store.get_var_mod_by_proc(proc);
}

std::unordered_set<std::string> PKB::get_proc_mod_by_var(std::string var) {
  return modify_store.get_proc_mod_by_var(var);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::get_all_mod_stmt_var() {
  return modify_store.get_all_stmt_var();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::get_all_mod_proc_var() {
  return modify_store.get_all_proc_var();
}

/* Checkers */

bool PKB::is_usage_stmt_var_exist(std::pair<std::string, std::string> pair) {
  return usage_store.stmt_var_exists(pair);
}

bool PKB::is_usage_proc_var_exist(std::pair<std::string, std::string> pair) {
  return usage_store.proc_var_exists(pair);
}

bool PKB::is_modify_stmt_var_exist(std::pair<std::string, std::string> pair) {
  return modify_store.stmt_var_exists(pair);
}

bool PKB::is_modify_proc_var_exist(std::pair<std::string, std::string> pair) {
  return modify_store.stmt_var_exists(pair);
}
