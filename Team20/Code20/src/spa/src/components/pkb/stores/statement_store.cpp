#include "statement_store.h"

StatementStore::StatementStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : Store(stmt_vector) {}

void StatementStore::AddStmtVar(std::string stmt, std::string var) {
  all_stmt.insert({stmt});
  stmt_var_pairs.emplace(std::pair<std::string, std::string>(stmt, var));

  if (!stmt_var_map.emplace(stmt, std::unordered_set<std::string>{var}).second) {
    stmt_var_map.at(stmt).emplace(var);
  }

  if (!var_stmt_map.emplace(var, std::unordered_set<std::string>{stmt}).second) {
    var_stmt_map.at(var).emplace(stmt);
  }
}

void StatementStore::AddProcVar(std::string proc, std::string var) {
  all_proc.insert({proc});
  proc_var_pairs.emplace(std::pair<std::string, std::string>(proc, var));

  if (!proc_var_map.emplace(proc, std::unordered_set<std::string>{var}).second) {
    proc_var_map.at(proc).emplace(var);
  }

  if (!var_proc_map.emplace(var, std::unordered_set<std::string>{proc}).second) {
    var_proc_map.at(var).emplace(proc);
  }
}

bool StatementStore::StmtVarExists(std::pair<std::string, std::string> pair) {
  return stmt_var_pairs.find(pair) != stmt_var_pairs.end();
}

bool StatementStore::ProcVarExists(std::pair<std::string, std::string> pair) {
  return proc_var_pairs.find(pair) != proc_var_pairs.end();
}

std::unordered_set<std::string> StatementStore::GetVarByStmt(std::string stmt) {
  if (stmt_var_map.find(stmt) != stmt_var_map.end()) {
    return stmt_var_map.at(stmt);
  }
  return {};
}

std::unordered_set<std::string> StatementStore::GetStmtByVar(std::string var) {
  if (var_stmt_map.find(var) != var_stmt_map.end()) {
    return var_stmt_map.at(var);
  }
  return {};
}

std::unordered_set<std::string> StatementStore::GetVarByProc(std::string proc) {
  if (proc_var_map.find(proc) != proc_var_map.end()) {
    return proc_var_map.at(proc);
  }
  return {};
}

std::unordered_set<std::string> StatementStore::GetProcByVar(std::string var) {
  if (var_proc_map.find(var) != var_proc_map.end()) {
    return var_proc_map.at(var);
  }
  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StatementStore::GetAllStmtVar() {
  return stmt_var_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StatementStore::GetAllProcVar() {
  return proc_var_pairs;
}

std::unordered_set<std::string> StatementStore::GetAllStmt() {
  return all_stmt;
}

std::unordered_set<std::string> StatementStore::GetAllProc() {
  return all_proc;
}
