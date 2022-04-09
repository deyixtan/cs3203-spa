#include "affect_session.h"
#include "../../../source_subsystem/types/cfg/cfg_node.h"

AffectSession::AffectSession(std::shared_ptr<AffectStore> affects_store, bool is_affect_star_involved) :
    m_affects_store(std::move(affects_store)),
    m_last_modified_star_map(std::unordered_map<std::string, std::unordered_set<std::string>>()),
    m_is_affect_star_involved(is_affect_star_involved) {
  TraverseCfg();
}

bool AffectSession::IsAffected(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_pairs.begin(), m_all_affects_pairs.end(), predicate);
}

bool AffectSession::IsAffectedStar(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_star_pairs.begin(), m_all_affects_star_pairs.end(), predicate);
}

bool AffectSession::IsAffecting(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_pairs.begin(), m_all_affects_pairs.end(), predicate);
}

bool AffectSession::IsAffectingStar(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_star_pairs.begin(), m_all_affects_star_pairs.end(), predicate);
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

// HELPER METHODS
std::string AffectSession::GetFollowingOf(std::string &stmt_no) {
  return m_affects_store->GetFollowsStore()->GetFollowingOf(STMT, stmt_no);
}

std::unordered_set<std::string> AffectSession::GetVarModByStmt(std::string &stmt_no) {
  return m_affects_store->GetModifyStore()->GetVarModByStmt(stmt_no);
}

std::unordered_set<std::string> AffectSession::GetVarUsedByStmt(std::string &stmt_no) {
  return m_affects_store->GetUsageStore()->GetVarUsedByStmt(stmt_no);
}

void AffectSession::TraverseCfg() {
  std::unordered_map<std::string, std::shared_ptr<source::CfgNode>>
      cfg_map = m_affects_store->GetProgramCfg()->GetCfgMap();
  for (auto const &cfg : cfg_map) {
    std::shared_ptr<source::CfgNode> proc_head = cfg.second;
    std::shared_ptr<source::CfgNode> tmp = std::make_shared<source::CfgNode>();
    std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map;
    m_last_modified_star_map_stack.push(std::make_shared<std::unordered_map<std::string, std::unordered_set<std::string>>>(last_modified_map));
    TraverseCfg(proc_head, tmp);
    m_last_modified_star_map_stack.pop();
  }
}

void AffectSession::TraverseCfg(std::shared_ptr<source::CfgNode> &cfg_node,
                                std::shared_ptr<source::CfgNode> &terminating_node) {
  // checks on TraverseCfg from If-statement handler
  if (!cfg_node->GetStatementList().empty() && !terminating_node->GetStatementList().empty()) {
    if (cfg_node->GetStatementList().front().stmt_no == terminating_node->GetStatementList().front().stmt_no) {
      terminating_node = cfg_node;
      return;
    }
  }

  // process statements in a cfg_node
  std::vector<source::CfgNodeStatement> statements = cfg_node->GetStatementList();
  for (auto const &stmt : statements) {
    std::string stmt_no = stmt.stmt_no;
    StmtType type = stmt.type;
    if (type == ASSIGN) {
      HandleAssignStatement(stmt_no);
    } else if (type == READ) {
      HandleReadStatement(stmt_no);
    } else if (type == CALL) {
      HandleCallStatement(stmt_no);
    } else if (type == WHILE) {
      HandleWhileStatement(cfg_node);
    } else if (type == IF) {
      HandleIfStatement(stmt_no, cfg_node, terminating_node);
      TraverseCfg(cfg_node, terminating_node);
      return;
    }
  }

  // check if there are no next nodes
  if (cfg_node->GetNextList().empty()) {
    return;
  }

  std::shared_ptr<source::CfgNode> next_node = cfg_node->GetNextList().back();
  TraverseCfg(next_node, terminating_node);
}

void AffectSession::HandleAssignStatement(std::string stmt_no) {
  // check variables being used if they are modified
  // add them to affects map
  std::unordered_set<std::string> vars_used = GetVarUsedByStmt(stmt_no);
  for (auto const &var_used : vars_used) {
    // check if var_used is modified before
    if (m_last_modified_star_map_stack.top()->count(var_used) != 0) {
      std::unordered_set<std::string> last_mod_stmt_nos = m_last_modified_star_map_stack.top()->at(var_used);
      for (auto const &last_mod_stmt_no : last_mod_stmt_nos) {
        // update affects (m_affects_map)
        if (m_affects_map.count(last_mod_stmt_no) == 0) {
          m_affects_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_map.at(last_mod_stmt_no).insert(stmt_no);
        // update affects (m_affects_reverse_map)
        if (m_affects_reverse_map.count(stmt_no) == 0) {
          m_affects_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_reverse_map.at(stmt_no).insert(last_mod_stmt_no);
        // update affects (m_all_affects_pairs)
        m_all_affects_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
        // update affects same synonym
        if (last_mod_stmt_no == stmt_no) {
          m_same_affects_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
        }

        if (m_is_affect_star_involved) {
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
          if (last_mod_stmt_no == stmt_no) {
            m_same_affects_star_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
          }

          if (m_last_modified_star_map.count(stmt_no) == 0) {
            m_last_modified_star_map.insert({stmt_no, std::unordered_set<std::string>()});
          }
          m_last_modified_star_map.at(stmt_no).insert(last_mod_stmt_no);

          for (const auto &p : m_last_modified_star_map[last_mod_stmt_no]) {
            if (m_affects_star_map.count(p) == 0) {
              m_affects_star_map.insert({p, std::unordered_set<std::string>()});
            }
            m_affects_star_map.at(p).insert(stmt_no);
            // update affects star (m_affects_star_reverse_map)
            if (m_affects_star_reverse_map.count(stmt_no) == 0) {
              m_affects_star_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
            }
            m_affects_star_reverse_map.at(stmt_no).insert(p);
            // update affects star (m_all_affects_star_pairs)
            m_all_affects_star_pairs.insert(std::make_pair(p, stmt_no));
            if (p == stmt_no) {
              m_same_affects_star_pairs.insert(std::make_pair(p, stmt_no));
            }

            if (m_last_modified_star_map.count(stmt_no) == 0) {
              m_last_modified_star_map.insert({stmt_no, std::unordered_set<std::string>()});
            }
            m_last_modified_star_map.at(stmt_no).insert(p);
          }
        }
      }
    }
  }

  // update modified table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (auto const &var_mod : vars_mod) {
    if (m_last_modified_star_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_star_map_stack.top()->insert({var_mod, std::unordered_set<std::string>()});
    }
    m_last_modified_star_map_stack.top()->at(var_mod).clear();
    m_last_modified_star_map_stack.top()->at(var_mod).insert(stmt_no);
  }
}

void AffectSession::HandleReadStatement(std::string stmt_no) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("READ SHOULD ONLY HAVE ONE VAR MOD");
  }

  std::string var_mod = *(vars_mod.begin());

  if (m_last_modified_star_map_stack.top()->count(var_mod) == 0) {
    m_last_modified_star_map_stack.top()->insert({var_mod, std::unordered_set<std::string>()});
  }
  m_last_modified_star_map_stack.top()->at(var_mod).clear();
}

void AffectSession::HandleCallStatement(std::string stmt_no) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (auto const &var_mod : vars_mod) {
    if (m_last_modified_star_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_star_map_stack.top()->insert({var_mod, std::unordered_set<std::string>()});
    }
    m_last_modified_star_map_stack.top()->at(var_mod).clear();
  }
}

void AffectSession::HandleWhileStatement(std::shared_ptr<source::CfgNode> &cfg_node) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::shared_ptr<std::unordered_map<std::string, std::unordered_set<std::string>>> last_modified_map_clone = std::make_shared<std::unordered_map<std::string, std::unordered_set<std::string>>>(*(m_last_modified_star_map_stack.top()));
  std::shared_ptr<source::CfgNode> start_node = cfg_node->GetNextList().front();
  std::shared_ptr<source::CfgNode> end_node = cfg_node;

  // traverse twice to populate modified table properly
  m_last_modified_star_map_stack.push(last_modified_map_clone);

  TraverseCfg(start_node, end_node);
  int affect_star_pairs_size_prev = GetAffectsStarPairs().size();
  TraverseCfg(start_node, end_node);
  int affect_star_pairs_size_curr = GetAffectsStarPairs().size();

  while (affect_star_pairs_size_curr != affect_star_pairs_size_prev) {
    affect_star_pairs_size_prev = affect_star_pairs_size_curr;
    TraverseCfg(start_node, end_node);
    affect_star_pairs_size_curr = GetAffectsStarPairs().size();
  }
  m_last_modified_star_map_stack.pop();

  //
  // merge last_modified_map_clone into last_modified_map
  for (auto const &last_modified : *(last_modified_map_clone)) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (m_last_modified_star_map_stack.top()->count(var_name) == 0) {
      m_last_modified_star_map_stack.top()->insert({var_name, std::unordered_set<std::string>()});
    }
    m_last_modified_star_map_stack.top()->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }
}

void AffectSession::HandleIfStatement(std::string stmt_no,
                                      std::shared_ptr<source::CfgNode> &cfg_node,
                                      std::shared_ptr<source::CfgNode> &terminating_node) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::shared_ptr<std::unordered_map<std::string, std::unordered_set<std::string>>> last_modified_map_clone = std::make_shared<std::unordered_map<std::string, std::unordered_set<std::string>>>(*(m_last_modified_star_map_stack.top()));

  std::shared_ptr<source::CfgNode> if_cfg_node = cfg_node->GetNextList()[0];
  std::shared_ptr<source::CfgNode> else_cfg_node = cfg_node->GetNextList()[1];
  std::shared_ptr<source::CfgNode> end_node = std::make_shared<source::CfgNode>();
  std::string end_node_stmt_no = GetFollowingOf(stmt_no);
  end_node->AddStatement(StmtType::STMT, end_node_stmt_no);

  // first TraverseIfCfg (if-block) will process statements and find dummy node
  // for the second TraverseIfCfg (else-block)
  TraverseCfg(if_cfg_node, end_node);

  m_last_modified_star_map_stack.push(last_modified_map_clone);
  TraverseCfg(else_cfg_node, end_node);
  m_last_modified_star_map_stack.pop();

  // merge last_modified_map_clone into last_modified_map
  for (auto const &last_modified : *(last_modified_map_clone)) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (m_last_modified_star_map_stack.top()->count(var_name) == 0) {
      m_last_modified_star_map_stack.top()->insert({var_name, std::unordered_set<std::string>()});
    }
    m_last_modified_star_map_stack.top()->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  cfg_node = end_node;
}
