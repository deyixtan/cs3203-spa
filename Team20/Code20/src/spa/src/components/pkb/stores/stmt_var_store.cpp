#include "stmt_var_store.h"

StmtVarStore::StmtVarStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : Store(
    move(stmt_vector)) {}

void StmtVarStore::AddStmtVar(StmtType type, std::string stmt, std::string var) {
  all_stmt.insert({stmt});
  stmt_var_pairs.emplace(std::pair<std::string, std::string>(stmt, var));

  if (!stmt_var_map.emplace(stmt, std::unordered_set<std::string>{var}).second) {
    stmt_var_map.at(stmt).emplace(var);
  }

  if (var_stmt_map.find(var) != var_stmt_map.end()) {
    if (var_stmt_map.at(var).find(type) != var_stmt_map.at(var).end()) {
      var_stmt_map.at(var).at(type).insert(stmt);
    } else {
      var_stmt_map.at(var).insert({type, std::unordered_set<std::string>{stmt}});
    }
  } else {
    var_stmt_map.insert({var, {}});
    var_stmt_map.at(var).insert({type, std::unordered_set<std::string>{stmt}});
  }
}

void StmtVarStore::AddProcVar(std::string proc, std::string var) {
  all_proc.insert({proc});
  proc_var_pairs.emplace(std::pair<std::string, std::string>(proc, var));

  if (!stmt_var_map.emplace(proc, std::unordered_set<std::string>{var}).second) {
    stmt_var_map.at(proc).emplace(var);
  }

/*  if (!var_stmt_map.emplace(var, std::unordered_set<std::string>{proc}).second) {
    var_stmt_map.at(var).emplace(proc);
  }*/
}

/*void StmtVarStore::AddVarHelper(std::string index,
                                std::string var,
                                std::unordered_set<std::pair<std::string, std::string>, pair_hash> &index_pair,
                                std::unordered_map<std::string, std::unordered_set<std::string>> &var_map,
                                std::unordered_map<std::string,
                                                   std::unordered_set<std::string>> &reverse_var_map) {
  index_set.insert({index});
  index_pair.emplace(std::pair<std::string, std::string>(index, var));

  if (!var_map.emplace(index, std::unordered_set<std::string>{var}).second) {
    var_map.at(index).emplace(var);
  }

  if (!reverse_var_map.emplace(var, std::unordered_set<std::string>{index}).second) {
    reverse_var_map.at(var).emplace(index);
  }
}*/

bool StmtVarStore::IsStmtVarValid(std::pair<std::string, std::string> const &pair) {
  return stmt_var_pairs.find(pair) != stmt_var_pairs.end();
}

std::unordered_set<std::string> StmtVarStore::GetVarByStmt(std::string const &stmt) {
  if (stmt_var_map.find(stmt) != stmt_var_map.end()) {
    return stmt_var_map.at(stmt);
  }
  return {};
}

std::unordered_set<std::string> StmtVarStore::GetStmtByVar(StmtType type, std::string const &var) {
  if (var_stmt_map.find(var) != var_stmt_map.end()) {
    if (var_stmt_map.at(var).find(type) != var_stmt_map.at(var).end()) {
      return var_stmt_map.at(var).at(type);
    }
  }
  return {};
}

std::unordered_set<std::string> StmtVarStore::GetAllStmt() {
  return all_stmt;
}

std::unordered_set<std::string> StmtVarStore::GetAllProc() {
  return all_proc;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtVarStore::GetAllStmtVar() {
  return stmt_var_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtVarStore::GetAllProcVar() {
  return proc_var_pairs;
}