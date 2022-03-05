#include "pkb.h"

PKB::PKB() {
  follow_store = FollowStore();
  modify_store = ModifyStore();
  parent_store = ParentStore();
  usage_store = UsageStore();
}

FollowStore &PKB::GetFollowStore() {
  return follow_store;
}

ModifyStore &PKB::GetModifyStore() {
  return modify_store;
}
ParentStore &PKB::GetParentStore() {
  return parent_store;
}
UsageStore &PKB::GetUsageStore() {
  return usage_store;
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
    if (rhs != "_" && rhs.find("_") != std::string::npos) {
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

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  switch (type) {
    case STMT:return stmt_list;
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
    default:break;
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
    default:break;
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
    default:break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStmt(StmtType type1,
                                                                                         StmtType type2) {
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
    default:break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStarStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      follow_star_stmt_list = follow_store.GetFollowStarPairs();
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
    default:break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllFollowStarStmt(StmtType type1,
                                                                                             StmtType type2) {
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
    default:break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStmt(StmtType type) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      parent_child_list = parent_store.GetParentChildPairs();
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
    default:break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStmt(StmtType type1,
                                                                                         StmtType type2) {
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
    default:break;
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
    default:break;
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetAllParentStarStmt(StmtType type1,
                                                                                             StmtType type2) {
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
    default:break;
  }
}
