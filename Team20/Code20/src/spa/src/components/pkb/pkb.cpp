#include <vector>

#include "pkb.h"
#include <string>
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

PKB* PKB::GetInstance() {
  if (!instance) {
    instance = new PKB;
  }
  return instance;
}

/* Adders */
void PKB::AddStmt(std::string stmt, StmtType type) {
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

void PKB::AddUsageStmtVar(std::string stmt, std::string var) {
  usage_store.AddStmtVar(stmt, var);
}

void PKB::AddUsageProcVar(std::string proc, std::string var) {
  usage_store.AddProcVar(proc, var);
}

void PKB::AddModifyStmtVar(std::string stmt, std::string var) {
  modify_store.AddStmtVar(stmt, var);
}

void PKB::AddModifyProcVar(std::string proc, std::string var) {
  modify_store.AddProcVar(proc, var);
}

void PKB::AddPattern(std::string stmt, std::string lhs, std::string rhs) {
  pattern_map[{lhs, rhs}] = stmt;
}

std::unordered_set<std::string> PKB::GetStmtWithPattern(std::string lhs, std::string rhs) {
  std::unordered_set<std::string> result = {};
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());
  rhs = "(" + rhs + ")";
  //TODO: Tree builder and parser

  for (auto const& [key, val] : pattern_map) {
    if (lhs == "_" && rhs == "_") {
      result.insert(val);
    }

    if (lhs == "_" && rhs != "_") {
      if (key.second.find(rhs) != -1) {
        result.insert(val);
      }
    }

    if (lhs != "_" && rhs == "_") {
      if (lhs == key.first) {
        result.insert(val);
      }
    }

    if (lhs != "_" && rhs != "_") {
      if (lhs == key.first && key.second.find(rhs) != -1) {
        result.insert(val);
      }
    }
  }

  return result;
}

/* Getters */

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
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

std::unordered_set<std::string> PKB::GetVarUsedByStmt(std::string stmt) {
  return usage_store.GetVarUsedByStmt(stmt);
}

std::unordered_set<std::string> PKB::GetStmtUsedByVar(std::string var) {
  return usage_store.GetStmtUsedByVar(var);
}

std::unordered_set<std::string> PKB::GetVarUsedByProc(std::string proc) {
  return usage_store.GetVarUsedByProc(proc);
}

std::unordered_set<std::string> PKB::GetProcUsedByVar(std::string var) {
  return usage_store.GetProcUsedByVar(var);
}

std::unordered_set<std::string> PKB::GetAllStmtUsing() {
  return usage_store.GetAllStmtUsing();
}

std::unordered_set<std::string> PKB::GetAllProcUsing() {
  return usage_store.GetAllProcUsing();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllUsageStmtVar() {
  return usage_store.GetAllStmtVar();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllUsageProcVar() {
  return usage_store.GetAllProcVar();
}

std::unordered_set<std::string> PKB::GetVarModByStmt(std::string stmt) {
  return modify_store.GetVarModByStmt(stmt);
}

std::unordered_set<std::string> PKB::GetStmtModByVar(std::string var) {
  return modify_store.GetStmtModByVar(var);
}

std::unordered_set<std::string> PKB::GetVarModByProc(std::string proc) {
  return modify_store.GetVarModByProc(proc);
}

std::unordered_set<std::string> PKB::GetProcModByVar(std::string var) {
  return modify_store.GetProcModByVar(var);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllModStmtVar() {
  return modify_store.GetAllStmtVar();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllModProcVar() {
  return modify_store.GetAllProcVar();
}

/* Checkers */

bool PKB::IsUsageStmtVarExist(std::pair<std::string, std::string> pair) {
  return usage_store.StmtVarExists(pair);
}

bool PKB::IsUsageProcVarExist(std::pair<std::string, std::string> pair) {
  return usage_store.ProcVarExists(pair);
}

bool PKB::IsModifyStmtVarExist(std::pair<std::string, std::string> pair) {
  return modify_store.StmtVarExists(pair);
}

bool PKB::IsModifyProcVarExist(std::pair<std::string, std::string> pair) {
  return modify_store.ProcVarExists(pair);
}
