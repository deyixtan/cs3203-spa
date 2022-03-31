#include "stmt_var_store.h"

StmtVarStore::StmtVarStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : Store(
    move(stmt_vector)) {}

void StmtVarStore::AddStmtVar(StmtType type, std::string stmt, std::string var) {
  if (type == PROC) {
    all_proc.insert({stmt});
  } else {
    all_stmt.insert({stmt});
  }

  if (!stmt_var_map.emplace(stmt, std::unordered_set<std::string>{var}).second) {
    stmt_var_map.at(stmt).emplace(var);
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> set = {};
  if (type_pair_map.find(type) != type_pair_map.end()) {
    type_pair_map.at(type).emplace(std::pair<std::string, std::string>(stmt, var));
  } else {
    set.insert(std::pair<std::string, std::string>(stmt, var));
    type_pair_map.insert({type, set});
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

bool StmtVarStore::IsStmtVarValid(std::pair<std::string, std::string> const &pair) {
  if (stmt_var_map.find(pair.first) != stmt_var_map.end()) {
    if (stmt_var_map.at(pair.first).find(pair.second) != stmt_var_map.at(pair.first).end()) {
      return true;
    }
  }
  return false;
}

std::unordered_set<std::string> StmtVarStore::GetVarByStmt(std::string const &stmt) {
  if (stmt_var_map.find(stmt) != stmt_var_map.end()) {
    return stmt_var_map.at(stmt);
  }
  return {};
}

std::unordered_set<std::string> StmtVarStore::GetStmtByVar(StmtType type, std::string const &var) {
  bool is_present = false;

  if (var_stmt_map.find(var) != var_stmt_map.end()) {
    if (var_stmt_map.at(var).find(type) != var_stmt_map.at(var).end()) {
      is_present = true;
    }
  }

  if (is_present && type == STMT) {
    std::unordered_map<StmtType, std::unordered_set<std::string>> set = var_stmt_map.at(var);
    for (auto &i : set) {

    }
  }

  if (is_present && type != STMT) {
    return var_stmt_map.at(var).at(type);
  }

  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> StmtVarStore::GetPairByType(StmtType type) {
  if (type_pair_map.find(type) != type_pair_map.end()) {
    return type_pair_map.at(type);
  }
  return {};
}

std::unordered_set<std::string> StmtVarStore::GetAllStmt() {
  return all_stmt;
}

std::unordered_set<std::string> StmtVarStore::GetAllProc() {
  return all_proc;
}