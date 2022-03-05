#include <vector>

#include "pkb.h"
#include <string>

PKB::PKB() {
  follow_store = FollowStore();
  parent_store = ParentStore();
  usage_store = UsageStore();
  modify_store = ModifyStore();
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
  pattern_map[stmt] = {lhs, rhs};
}

std::unordered_set<std::string> PKB::GetStmtWithPattern(std::string lhs, std::string rhs) {
  std::unordered_set<std::string> result = {};
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());
  //rhs = "(" + rhs + ")";
  //TODO: Tree builder and parser

  for (auto const&[key, val] : pattern_map) {
    if (lhs == "_" && rhs == "_") {
      result.insert(key);
    }

    if (lhs == "_" && rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    if (lhs == "_" && rhs != "_" && rhs.find("_") == std::string::npos) {
      if (val.second == rhs) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs == "_") {
      if (lhs == val.first) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (lhs == val.first && val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs != "_" && rhs.find("_") == std::string::npos) {
      if (lhs == val.first && val.second == rhs) {
        result.insert(key);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetStmtWithPatternSynonym(std::string rhs) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());

  for (auto const&[key, val] : pattern_map) {
    if (rhs == "_") {
      result.insert({key, val.first});
    }

    // Exact match
    if (rhs != "_" && rhs.find("_") == std::string::npos) {
      if (val.second == rhs) {
        result.insert({key, val.first});
      }
    }

    // Partial match
    if (rhs != "_" && rhs.find("_") != std::string::npos){
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert({key, val.first});
      }
    }
  }

  return result;
}

std::string PKB::GetFollowingOf(std::string stmt) {
  return follow_store.GetFollowingOf(stmt);
}

std::unordered_set<std::string> PKB::GetFollowingStarOf(std::string stmt) {
  return follow_store.GetFollowingStarOf(stmt);
}

std::string PKB::GetFollowerOf(std::string stmt) {
  return follow_store.GetFollowerOf(stmt);
}

std::unordered_set<std::string> PKB::GetFollowerStarOf(std::string stmt) {
  return follow_store.GetFollowerStarOf(stmt);
}

bool PKB::IsFollowExist(std::pair<std::string, std::string> pair) {
  return follow_store.FollowExists(pair);
}

bool PKB::IsFollowStarExist(std::pair<std::string, std::string> pair) {
  return follow_store.FollowStarExists(pair);
}

bool PKB::IsFollower(std::string stmt) {
  return follow_store.IsFollower(stmt);
}

bool PKB::IsFollowing(std::string stmt) {
  return follow_store.IsFollowing(stmt);
}

bool PKB::IsFollowerStar(std::string stmt) {
  return follow_store.IsFollowerStar(stmt);
}

bool PKB::IsFollowingStar(std::string stmt) {
  return follow_store.IsFollowingStar(stmt);
}

std::string PKB::GetParentOf(std::string stmt) {
  return parent_store.GetParentOf(stmt);
}

std::unordered_set<std::string> PKB::GetAnceOf(std::string stmt) {
  return parent_store.GetAllAnceOf(stmt);
}

std::unordered_set<std::string> PKB::GetChildOf(std::string stmt) {
  return parent_store.GetChildOf(stmt);
}

std::unordered_set<std::string> PKB::GetDescOf(std::string stmt) {
  return parent_store.GetAllDescOf(stmt);
}

bool PKB::IsParent(std::string stmt) {
  return parent_store.IsParent(stmt);
}

bool PKB::IsChild(std::string stmt) {
  return parent_store.IsChild(stmt);
}

bool PKB::IsAnce(std::string stmt) {
  return parent_store.IsAnce(stmt);
}

bool PKB::IsDesc(std::string stmt) {
  return parent_store.IsDesc(stmt);
}

bool PKB::ParentChildExists(std::string stmt1, std::string stmt2) {
  return parent_store.ParentChildExists(stmt1, stmt2);
}

bool PKB::AnceExists(std::string curr, std::string ance) {
  return parent_store.AnceExists(curr, ance);
}

bool DescExists(std::string curr, std::string desc) {

}

void PKB::AddFollowStmt(std::string stmt1, std::string stmt2) {
  follow_store.AddFollow(stmt1, stmt2);
}

void PKB::AddFollowStarStmt(std::string stmt1, std::string stmt2) {
  follow_store.AddFollowStar(stmt1, stmt2);
}

void PKB::AddParentStmt(std::string stmt1, std::string stmt2) {
  parent_store.AddParentStmt(stmt1, stmt2);
}

void PKB::AddParentStarStmt(std::string stmt, std::vector<std::string> visited) {
  parent_store.AddParentStarStmt(stmt, visited);
}

/* Getters */

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  switch (type) {
    case STMT:
      return stmt_list;
    case WHILE:
      return while_stmt_list;
    case READ:
      return read_stmt_list;
    case PRINT:
      return print_stmt_list;
    case CALL:
      return call_stmt_list;
    case IF:
      return if_stmt_list;
    case ASSIGN:
      return assign_stmt_list;
    case PROC:
      return proc_list;
    case VARS:
      return var_list;
    case CONSTS:
      return const_list;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllModStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> mod_stmt_var_list = modify_store.GetAllStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type) {
    case STMT:
      for (auto i : mod_stmt_var_list) {
        for (auto j : stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : mod_stmt_var_list) {
        for (auto j : read_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : mod_stmt_var_list) {
        for (auto j : assign_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : mod_stmt_var_list) {
        for (auto j : if_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : mod_stmt_var_list) {
        for (auto j : while_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllUsesStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> uses_stmt_list = usage_store.GetAllStmtVar();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type) {
    case STMT:
      for (auto i : uses_stmt_list) {
        for (auto j : stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : uses_stmt_list) {
        for (auto j : while_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : uses_stmt_list) {
        for (auto j : print_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : uses_stmt_list) {
        for (auto j : if_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : uses_stmt_list) {
        for (auto j : assign_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_stmt_list = follow_store.GetFollowPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type) {
    case STMT:
      for (auto i : follow_stmt_list) {
        for (auto j : stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : follow_stmt_list) {
        for (auto j : read_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : follow_stmt_list) {
        for (auto j : assign_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : follow_stmt_list) {
        for (auto j : while_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : follow_stmt_list) {
        for (auto j : print_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : follow_stmt_list) {
        for (auto j : if_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStmt(StmtType type1, StmtType type2) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type1) {
    case STMT:
      for (auto i : GetAllFollowStmt(type2)) {
        for (auto j : stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : GetAllFollowStmt(type2)) {
        for (auto j : read_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : GetAllFollowStmt(type2)) {
        for (auto j : assign_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : GetAllFollowStmt(type2)) {
        for (auto j : while_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : GetAllFollowStmt(type2)) {
        for (auto j : print_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : GetAllFollowStmt(type2)) {
        for (auto j : if_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStarStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_star_stmt_list = follow_store.GetFollowStarPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type) {
    case STMT:
      for (auto i : follow_star_stmt_list) {
        for (auto j : stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : follow_star_stmt_list) {
        for (auto j : read_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : follow_star_stmt_list) {
        for (auto j : assign_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : follow_star_stmt_list) {
        for (auto j : while_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : follow_star_stmt_list) {
        for (auto j : print_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : follow_star_stmt_list) {
        for (auto j : if_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStarStmt(StmtType type1, StmtType type2) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type1) {
    case STMT:
      for (auto i : GetAllFollowStarStmt(type2)) {
        for (auto j : stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : GetAllFollowStarStmt(type2)) {
        for (auto j : read_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : GetAllFollowStarStmt(type2)) {
        for (auto j : assign_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : GetAllFollowStarStmt(type2)) {
        for (auto j : while_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : GetAllFollowStarStmt(type2)) {
        for (auto j : print_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : GetAllFollowStarStmt(type2)) {
        for (auto j : if_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> parent_child_list = parent_store.GetParentChildPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type) {
    case STMT:
      for (auto i : parent_child_list) {
        for (auto j : stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : parent_child_list) {
        for (auto j : assign_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : parent_child_list) {
        for (auto j : print_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : parent_child_list) {
        for (auto j : read_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : parent_child_list) {
        for (auto j : if_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : parent_child_list) {
        for (auto j : while_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStmt(StmtType type1, StmtType type2) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type1) {
    case STMT:
      for (auto i : GetAllParentStmt(type2)) {
        for (auto j : stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : GetAllParentStmt(type2)) {
        for (auto j : if_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : GetAllParentStmt(type2)) {
        for (auto j : while_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStarStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> ance_desc_list = parent_store.GetAnceDescPairs();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type) {
    case STMT:
      for (auto i : ance_desc_list) {
        for (auto j : stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case ASSIGN:
      for (auto i : ance_desc_list) {
        for (auto j : assign_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case PRINT:
      for (auto i : ance_desc_list) {
        for (auto j : print_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case READ:
      for (auto i : ance_desc_list) {
        for (auto j : read_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : ance_desc_list) {
        for (auto j : if_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : ance_desc_list) {
        for (auto j : while_stmt_list) {
          if (i.second == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStarStmt(StmtType type1, StmtType type2) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;

  switch (type1) {
    case STMT:
      for (auto i : GetAllParentStarStmt(type2)) {
        for (auto j : stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case IF:
      for (auto i : GetAllParentStarStmt(type2)) {
        for (auto j : if_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    case WHILE:
      for (auto i : GetAllParentStarStmt(type2)) {
        for (auto j : while_stmt_list) {
          if (i.first == j) {
            result.insert(i);
          }
        }
      }
      return result;
    default:
      break;
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

std::unordered_set<std::string> PKB::GetAllStmtModify() {
  return modify_store.GetAllStmtModify();
}

std::unordered_set<std::string> PKB::GetAllProcModify() {
  return modify_store.GetAllProcModify();
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
