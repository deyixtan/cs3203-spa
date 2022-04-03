#include "affect_session.h"
#include "../../../source_subsystem/types/cfg/cfg_node.h"

AffectSession::AffectSession(std::shared_ptr<AffectStore> affects_store) : m_affects_store(affects_store) {
  TraverseCfg();
}

bool AffectSession::IsAffected(std::string const &stmt) {
  for (auto pairs : m_all_affects_pairs) {
    if (pairs.second == stmt) {
      return true;
    }
  }
  return false;
}

bool AffectSession::IsAffectedStar(std::string const &stmt) {
  for (auto pairs : m_all_affects_star_pairs) {
    if (pairs.second == stmt) {
      return true;
    }
  }
  return false;
}

bool AffectSession::IsAffecting(std::string const &stmt) {
  for (auto pairs : m_all_affects_pairs) {
    if (pairs.first == stmt) {
      return true;
    }
  }
  return false;
}

bool AffectSession::IsAffectingStar(std::string const &stmt) {
  for (auto pairs : m_all_affects_star_pairs) {
    if (pairs.first == stmt) {
      return true;
    }
  }
  return false;
}

bool AffectSession::DoesAffectExists(std::pair<std::string, std::string> const &pair) {
  if (m_all_affects_pairs.find(pair) != m_all_affects_pairs.end()) {
    return true;
  }
  return false;
}

bool AffectSession::DoesAffectStarExists(std::pair<std::string, std::string> const &pair) {
  if (m_all_affects_star_pairs.find(pair) != m_all_affects_star_pairs.end()) {
    return true;
  }
  return false;
}

std::unordered_set<std::string> AffectSession::GetAffectedOf(std::string const &stmt) {
  if (m_affects_map.count(stmt) == 0) {
    return {};
  }
  return m_affects_map.at(stmt);
}

std::unordered_set<std::string> AffectSession::GetAffectedStarOf(std::string const &stmt) {
  if (m_affects_star_map.count(stmt) == 0) {
    return {};
  }
  return m_affects_star_map.at(stmt);
}

std::unordered_set<std::string> AffectSession::GetAffectsOf(std::string const &stmt) {
  if (m_affects_reverse_map.count(stmt) == 0) {
    return {};
  }
  return m_affects_reverse_map.at(stmt);
}

std::unordered_set<std::string> AffectSession::GetAffectsStarOf(std::string const &stmt) {
  if (m_affects_star_reverse_map.count(stmt) == 0) {
    return {};
  }
  return m_affects_star_reverse_map.at(stmt);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAffectsPairs() {
  return m_all_affects_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAffectsStarPairs() {
  return m_all_affects_star_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAffectsSameSynPairs() {
  return m_same_affects_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAffectsStarSameSynPairs() {
  return m_same_affects_star_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAllAffectsStmt(StmtType type) {
  return m_affects_store->GetAllAffectsStmtHelper(m_all_affects_pairs, type);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAllAffectsStarStmt(StmtType type) {
  return m_affects_store->GetAllAffectsStmtHelper(m_all_affects_star_pairs, type);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAllAffectsStmt(StmtType type1,
                                                                                                    StmtType type2) {
  return m_affects_store->GetAllAffectsStmtHelper(m_all_affects_pairs, type1, type2);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> AffectSession::GetAllAffectsStarStmt(StmtType type1,
                                                                                                        StmtType type2) {
  return m_affects_store->GetAllAffectsStmtHelper(m_all_affects_star_pairs, type1, type2);
}

// HELPER METHODS
std::string AffectSession::GetFollowingOf(std::string stmt_no) {
  return m_affects_store->GetFollowsStore()->GetFollowingOf(STMT, stmt_no);
}

std::unordered_set<std::string> AffectSession::GetVarModByStmt(std::string stmt_no) {
  return m_affects_store->GetModifyStore()->GetVarModByStmt(stmt_no);
}

std::unordered_set<std::string> AffectSession::GetVarUsedByStmt(std::string stmt_no) {
  return m_affects_store->GetUsageStore()->GetVarUsedByStmt(stmt_no);
}

void AffectSession::TraverseCfg() {
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfg_map = m_affects_store->GetProgramCfg()->GetCfgMap();
  for (auto const &cfg : cfg_map) {
    std::shared_ptr<CfgNode> proc_head = cfg.second;
    std::shared_ptr<CfgNode> tmp = std::make_shared<CfgNode>();
    std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map;
    std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_star_map;
    TraverseCfg(proc_head, tmp, last_modified_map, last_modified_star_map);
  }
}

void AffectSession::TraverseCfg(std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_star_map) {
  // checks on TraverseCfg from If-statement handler
  if (cfg_node->GetStatementList().size() > 0 && terminating_node->GetStatementList().size() > 0) {
    if (cfg_node->GetStatementList().front().stmt_no == terminating_node->GetStatementList().front().stmt_no) {
      terminating_node = cfg_node;
      return;
    }
  }

  // process statements in a cfg_node
  std::vector<Statement> statements = cfg_node->GetStatementList();
  for (auto const &stmt : statements) {
    std::string stmt_no = stmt.stmt_no;
    StmtType type = stmt.type;
    if (type == ASSIGN) {
      HandleAssignStatement(stmt_no, last_modified_map, last_modified_star_map);
    } else if (type == READ) {
      HandleReadStatement(stmt_no, last_modified_map, last_modified_star_map);
    } else if (type == CALL) {
      HandleCallStatement(stmt_no, last_modified_map, last_modified_star_map);
    } else if (type == WHILE) {
      HandleWhileStatement(cfg_node, last_modified_map, last_modified_star_map);
    } else if (type == IF) {
      HandleIfStatement(stmt_no, cfg_node, terminating_node, last_modified_map, last_modified_star_map);
      TraverseCfg(cfg_node, terminating_node, last_modified_map, last_modified_star_map);
      return;
    }
  }

  // check if there are no next nodes
  if (cfg_node->GetDescendants().size() == 0) {
    return;
  }

  std::shared_ptr<CfgNode> next_node = cfg_node->GetDescendants().back();
  TraverseCfg(next_node, terminating_node, last_modified_map, last_modified_star_map);
}

void AffectSession::HandleAssignStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_star_map) {
  // check variables being used if they are modified
  // add them to affects map
  std::unordered_set<std::string> vars_used = GetVarUsedByStmt(stmt_no);
  for (std::string var_used : vars_used) {
    // check if var_used is modified before
    if (last_modified_map.count(var_used) != 0) {
      std::unordered_set<std::string> last_mod_stmt_nos = last_modified_map.at(var_used);
      for (auto const last_mod_stmt_no : last_mod_stmt_nos) {
        // update affects (m_affects_map)
        if (m_affects_map.count(last_mod_stmt_no) == 0) {
          m_affects_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_map.at(last_mod_stmt_no).insert(stmt_no);
        // update affects star (m_affects_star_map)
        if (m_affects_star_map.count(last_mod_stmt_no) == 0) {
          m_affects_star_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_star_map.at(last_mod_stmt_no).insert(stmt_no);

        // update affects (m_affects_reverse_map)
        if (m_affects_reverse_map.count(stmt_no) == 0) {
          m_affects_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_reverse_map.at(stmt_no).insert(last_mod_stmt_no);
        // update affects star (m_affects_star_reverse_map)
        if (m_affects_star_reverse_map.count(stmt_no) == 0) {
          m_affects_star_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_star_reverse_map.at(stmt_no).insert(last_mod_stmt_no);

        // update affects (m_all_affects_pairs)
        m_all_affects_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
        // update affects star (m_all_affects_star_pairs)
        m_all_affects_star_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));

        // update affects same synonym
        if (last_mod_stmt_no == stmt_no) {
          m_same_affects_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
        }

        // update modified star table
        std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
        for (auto const var_mod : vars_mod) {
          if (last_modified_star_map.count(var_mod) == 0) {
            last_modified_star_map.insert({var_mod, std::unordered_set<std::string>()});
          }
          //last_modified_star_map.at(var_mod).clear();
          last_modified_star_map.at(var_mod).insert(last_mod_stmt_no);
        }

        // add transitive closure
        if (last_modified_star_map.count(var_used) != 0) {
          std::unordered_set<std::string> last_mod_stmt_nos = last_modified_star_map.at(var_used);
          for (auto const last_mod_stmt_no : last_mod_stmt_nos) {
            // update affects star (m_affects_star_map)
            if (m_affects_star_map.count(last_mod_stmt_no) == 0) {
              m_affects_star_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
            }
            m_affects_star_map.at(last_mod_stmt_no).insert(stmt_no);
            // update affects star (m_affects_star_reverse_map)
            if (m_affects_star_reverse_map.count(stmt_no) == 0) {
              m_affects_star_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
            }
            m_affects_star_reverse_map.at(stmt_no).insert(last_mod_stmt_no);
            // update affects star (m_all_affects_star_pairs)
            m_all_affects_star_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));

            // update affects star same synonym
            if (last_mod_stmt_no == stmt_no) {
              m_same_affects_star_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
            }
          }
        }
      }
    }
  }

  // update modified table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (auto const var_mod : vars_mod) {
    if (last_modified_map.count(var_mod) == 0) {
      last_modified_map.insert({var_mod, std::unordered_set<std::string>()});
    }
    last_modified_map.at(var_mod).clear();
    last_modified_map.at(var_mod).insert(stmt_no);
  }
}

void AffectSession::HandleReadStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_star_map) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("READ SHOULD ONLY HAVE ONE VAR MOD");
  }

  std::string var_mod = *(vars_mod.begin());

  if (last_modified_map.count(var_mod) == 0) {
    last_modified_map.insert({var_mod, std::unordered_set<std::string>()});
  }
  last_modified_map.at(var_mod).clear();

  if (last_modified_star_map.count(var_mod) == 0) {
    last_modified_star_map.insert({var_mod, std::unordered_set<std::string>()});
  }
  last_modified_star_map.at(var_mod).clear();
}

void AffectSession::HandleCallStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_star_map) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (std::string var_mod : vars_mod) {
    if (last_modified_map.count(var_mod) == 0) {
      last_modified_map.insert({var_mod, std::unordered_set<std::string>()});
    }
    last_modified_map.at(var_mod).clear();

    if (last_modified_star_map.count(var_mod) == 0) {
      last_modified_star_map.insert({var_mod, std::unordered_set<std::string>()});
    }
    last_modified_star_map.at(var_mod).clear();
  }
}

void AffectSession::HandleWhileStatement(std::shared_ptr<CfgNode> &cfg_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_star_map) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map_clone = last_modified_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_star_map_clone = last_modified_star_map;
  std::shared_ptr<CfgNode> start_node = cfg_node->GetDescendants().front();
  std::shared_ptr<CfgNode> end_node = cfg_node;

  // traverse twice to populate modified table properly
  TraverseCfg(start_node, end_node, last_modified_map_clone, last_modified_star_map_clone);
  TraverseCfg(start_node, end_node, last_modified_map_clone, last_modified_star_map_clone);

  // merge last_modified_map_clone into last_modified_map
  for (auto last_modified : last_modified_map_clone) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (last_modified_map.count(var_name) == 0) {
      last_modified_map.insert({var_name, std::unordered_set<std::string>()});
    }
    last_modified_map.at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  for (auto last_modified_star : last_modified_star_map_clone) {
    std::string var_name = last_modified_star.first;
    std::unordered_set<std::string> stmt_nos = last_modified_star.second;
    if (last_modified_star_map.count(var_name) == 0) {
      last_modified_star_map.insert({var_name, std::unordered_set<std::string>()});
    }
    last_modified_star_map.at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }
}

void AffectSession::HandleIfStatement(std::string stmt_no, std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_star_map) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map_clone = last_modified_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_star_map_clone = last_modified_star_map;

  std::shared_ptr<CfgNode> if_cfg_node = cfg_node->GetDescendants()[0];
  std::shared_ptr<CfgNode> else_cfg_node = cfg_node->GetDescendants()[1];
  std::shared_ptr<CfgNode> end_node = std::make_shared<CfgNode>();
  std::string end_node_stmt_no = GetFollowingOf(stmt_no);
  end_node->AddStatement(StmtType::STMT, end_node_stmt_no);

  // first TraverseIfCfg (if-block) will process statements and find dummy node
  // for the second TraverseIfCfg (else-block)
  TraverseCfg(if_cfg_node, end_node, last_modified_map, last_modified_star_map);
  TraverseCfg(else_cfg_node, end_node, last_modified_map_clone, last_modified_star_map_clone);

  // merge last_modified_map_clone into last_modified_map
  for (auto last_modified : last_modified_map_clone) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (last_modified_map.count(var_name) == 0) {
      last_modified_map.insert({var_name, std::unordered_set<std::string>()});
    }
    last_modified_map.at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  for (auto last_modified_star : last_modified_star_map_clone) {
    std::string var_name = last_modified_star.first;
    std::unordered_set<std::string> stmt_nos = last_modified_star.second;
    if (last_modified_star_map.count(var_name) == 0) {
      last_modified_star_map.insert({var_name, std::unordered_set<std::string>()});
    }
    last_modified_star_map.at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  cfg_node = end_node;
}
