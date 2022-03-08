#include "statement_store.h"

StatementStore::StatementStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : Store(move(
    stmt_vector)) {}

void StatementStore::AddStmtVar(std::string stmt, std::string var) {
  AddVarHelper(stmt, var, all_stmt, stmt_var_pairs, stmt_var_map, var_stmt_map);
}

void StatementStore::AddProcVar(std::string proc, std::string var) {
  AddVarHelper(proc, var, all_proc, proc_var_pairs, proc_var_map, var_proc_map);
}

void StatementStore::AddVarHelper(std::string index,
                                  std::string var,
                                  std::unordered_set<std::string> &index_set,
                                  std::unordered_set<std::pair<std::string, std::string>, pair_hash> &index_pair,
                                  std::unordered_map<std::string, std::unordered_set<std::string>> &var_map,
                                  std::unordered_map<std::string, std::unordered_set<std::string>> &reverse_var_map) {
  index_set.insert({index});
  index_pair.emplace(std::pair<std::string, std::string>(index, var));

  if (!var_map.emplace(index, std::unordered_set<std::string>{var}).second) {
    var_map.at(index).emplace(var);
  }

  if (!reverse_var_map.emplace(var, std::unordered_set<std::string>{index}).second) {
    reverse_var_map.at(var).emplace(index);
  }
}

bool StatementStore::StmtVarExists(std::pair<std::string, std::string> const &pair) {
  return stmt_var_pairs.find(pair) != stmt_var_pairs.end();
}

bool StatementStore::ProcVarExists(std::pair<std::string, std::string> const &pair) {
  return proc_var_pairs.find(pair) != proc_var_pairs.end();
}

std::unordered_set<std::string> StatementStore::GetVarByStmt(std::string const &stmt) {
  if (stmt_var_map.find(stmt) != stmt_var_map.end()) {
    return stmt_var_map.at(stmt);
  }
  return {};
}

std::unordered_set<std::string> StatementStore::GetStmtByVar(std::string const &var) {
  if (var_stmt_map.find(var) != var_stmt_map.end()) {
    return var_stmt_map.at(var);
  }
  return {};
}

std::unordered_set<std::string> StatementStore::GetVarByProc(std::string const &proc) {
  if (proc_var_map.find(proc) != proc_var_map.end()) {
    return proc_var_map.at(proc);
  }
  return {};
}

std::unordered_set<std::string> StatementStore::GetProcByVar(std::string const &var) {
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