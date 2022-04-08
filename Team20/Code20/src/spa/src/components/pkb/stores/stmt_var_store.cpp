#include "stmt_var_store.h"

StmtVarStore::StmtVarStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                           std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) : Store(
    move(stmt_vector), move(stmt_type)) {}

void StmtVarStore::AddStmtVar(std::string stmt, std::string var) {
  StmtType type;

  if (isalpha(stmt.at(0))) {
    type = PROC;
  } else {
    type = m_stmt_type->at(stmt);
  }

  if (type == PROC) {
    all_proc.insert({stmt});
  } else {
    all_stmt.insert({stmt});
    AddStmtVarHelper(STMT, stmt, var);
  }

  AddStmtVarHelper(type, stmt, var);
}

void StmtVarStore::AddStmtVarHelper(StmtType type, std::string stmt, std::string var) {
  if (!stmt_var_map.emplace(stmt, std::unordered_set<std::string>{var}).second) {
    stmt_var_map.at(stmt).emplace(var);
  }

  IDENT_PAIR_SET set = {};
  if (type_pair_map.find(type) != type_pair_map.end()) {
    type_pair_map.at(type).push_back({stmt, var});
  } else {
    set.push_back({stmt, var});
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
      return var_stmt_map.at(var).at(type);
    }
  }

  return {};
}

IDENT_PAIR_SET StmtVarStore::GetPairByType(StmtType type) {
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