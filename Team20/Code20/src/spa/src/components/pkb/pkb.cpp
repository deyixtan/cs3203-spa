#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "pkb.h"
#include "components/source_subsystem/TNode.h"

/*
int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}
*/

//Keywords = [procedure, while, read, print, call, if-else-then, assign] -> Need to maintain 1 list for each of these

PKB::PKB() {}

/* Adders */

void PKB::add_stmt(int stmt, std::string name, StmtType type) {
  switch (type) {
    case PROC:proc_list.insert(name);
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
    case VARS:var_list.insert(name);
      break;
    case CONSTS:const_list.insert(name);
      break;
    default:break;
  }
}

bool PKB::add_usage_stmt_var(int stmt, std::string var) {
  usage_store.add_stmt_var(stmt, var);
}

bool PKB::add_usage_proc_var(std::string proc, std::string var) {
  usage_store.add_proc_var(proc, var);
}

bool PKB::add_modify_stmt_var(int stmt, std::string var) {
  modify_store.add_stmt_var(stmt, var);
}

bool PKB::add_modify_proc_var(std::string proc, std::string var) {
  modify_store.add_proc_var(proc, var);
}

/* Getters */

template<typename T>
T PKB::get_stmt(StmtType type) {
  switch (type) {
    case PROC:return proc_list;
    case WHILE:return while_stmt_list;
    case READ:return read_stmt_list;
    case PRINT:return print_stmt_list;
    case CALL:return call_stmt_list;
    case IF:return if_stmt_list;
    case ASSIGN:return assign_stmt_list;
    case VARS:return var_list;
    case CONSTS:return const_list;
    default:break;
  }
}
