#include "usage_store.h"

UsageStore::UsageStore() {}

void UsageStore::add_stmt_var(int stmt, std::string var) {
  stmt_var_pairs.emplace(std::pair<int, std::string>(stmt, var));

  if (!stmt_var_map.emplace(stmt, std::unordered_set<std::string>{ var }).second) {
    stmt_var_map.at(stmt).emplace(var);
  }

  if (!var_stmt_map.emplace(var, std::unordered_set<int>{ stmt }).second) {
    var_stmt_map.at(var).emplace(stmt);
  }
}

void UsageStore::add_proc_var(std::string proc, std::string var) {
  proc_var_pairs.emplace(std::pair<std::string, std::string>(proc, var));

  if (!proc_var_map.emplace(proc, std::unordered_set<std::string>{ var }).second) {
    proc_var_map.at(proc).emplace(var);
  }

  if (!proc_var_map.emplace(var, std::unordered_set<std::string>{ proc }).second) {
    proc_var_map.at(var).emplace(proc);
  }
}

bool UsageStore::stmt_var_exists(std::pair<int, std::string> pair) {
  return stmt_var_pairs.find(pair) != stmt_var_pairs.end();
}

bool UsageStore::proc_var_exists(std::pair<std::string, std::string> pair) {
  return proc_var_pairs.find(pair) != proc_var_pairs.end();
}

std::unordered_set<std::string> UsageStore::get_var_used_by_stmt(int stmt) {
  if (stmt_var_map.find(stmt) != stmt_var_map.end()) {
    return stmt_var_map.at(stmt);
  }
  return {};
}

std::unordered_set<int> UsageStore::get_stmt_used_by_var(std::string var) {
  if (var_stmt_map.find(var) != var_stmt_map.end()) {
    return var_stmt_map.at(var);
  }
  return {};
}

std::unordered_set<std::string> UsageStore::get_var_used_by_proc(std::string proc) {
  if (proc_var_map.find(proc) != proc_var_map.end()) {
    return proc_var_map.at(proc);
  }
  return {};
}

std::unordered_set<std::string> UsageStore::get_proc_used_by_var(std::string var) {
  if (var_proc_map.find(var) != var_proc_map.end()) {
    return var_proc_map.at(var);
  }
  return {};
}

std::unordered_set<std::pair<int, std::string>, pair_hash> UsageStore::get_all_stmt_var() {
  return stmt_var_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> UsageStore::get_all_proc_var() {
  return proc_var_pairs;
}
