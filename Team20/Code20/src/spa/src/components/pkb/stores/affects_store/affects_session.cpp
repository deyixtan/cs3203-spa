#include "affects_session.h"
#include "../../../source_subsystem/types/cfg/cfg_node.h"

AffectsSession::AffectsSession(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                               std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                               std::shared_ptr<AffectsStore> affects_store,
                               bool is_affects_star_involved) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)),
    m_affects_store(std::move(affects_store)),
    m_last_modified_star_map(std::unordered_map<std::string, std::unordered_set<std::string>>()),
    m_is_affects_star_involved(is_affects_star_involved),
    m_terminating_node_stack(std::stack<std::shared_ptr<source::CfgNode>>()) {
  TraverseCfg();
}

bool AffectsSession::IsAffected(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_pairs2.begin(), m_all_affects_pairs2.end(), predicate);
}

bool AffectsSession::IsAffectedStar(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_star_pairs2.begin(), m_all_affects_star_pairs2.end(), predicate);
}

bool AffectsSession::IsAffecting(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_pairs2.begin(), m_all_affects_pairs2.end(), predicate);
}

bool AffectsSession::IsAffectingStar(std::string const &stmt) {
  auto predicate = [stmt](const std::pair<std::string, std::string> &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_star_pairs2.begin(), m_all_affects_star_pairs2.end(), predicate);
}

bool AffectsSession::DoesAffectExists(IDENT_PAIR const &pair) {
  if (m_all_affects_pairs2.find(pair) != m_all_affects_pairs2.end()) {
    return true;
  }
//  for(IDENT_PAIR_VECTOR::iterator it = GetAllPairs().begin(); it != GetAllPairs().end(); ++it) {
//    std::string first = it->first;
//    std::string second = it->second;
//    if (first == pair.first && second == pair.second) {
//      return true;
//    }
//  }
  return false;
}

bool AffectsSession::DoesAffectStarExists(IDENT_PAIR const &pair) {
  if (m_all_affects_star_pairs2.find(pair) != m_all_affects_star_pairs2.end()) {
    return true;
  }
//  for (IDENT_PAIR_VECTOR::iterator it = GetAllStarPairs().begin(); it != GetAllStarPairs().end(); ++it) {
//    std::string first = it->first;
//    std::string second = it->second;
//    if (first == pair.first && second == pair.second) {
//      return true;
//    }
//  }
  return false;
}

std::unordered_set<std::string> AffectsSession::GetAffectedOf(std::string const &stmt) {
//  if (m_affects_map.count(stmt) == 0) {
//    return {};
//  }
//  return m_affects_map.at(stmt);
  return GetLowerSetOf(AFFECTS, ASSIGN, stmt);
}

std::unordered_set<std::string> AffectsSession::GetAffectedStarOf(std::string const &stmt) {
//  if (m_affects_star_map.count(stmt) == 0) {
//    return {};
//  }
//  return m_affects_star_map.at(stmt);
  return GetLowerStarOf(AFFECTS, ASSIGN, stmt);
}

std::unordered_set<std::string> AffectsSession::GetAffectsOf(std::string const &stmt) {
//  if (m_affects_reverse_map.count(stmt) == 0) {
//    return {};
//  }
//  return m_affects_reverse_map.at(stmt);
  return GetUpperSetOf(AFFECTS, ASSIGN, stmt);
}

std::unordered_set<std::string> AffectsSession::GetAffectsStarOf(std::string const &stmt) {
//  if (m_affects_star_reverse_map.count(stmt) == 0) {
//    return {};
//  }
//  return m_affects_star_reverse_map.at(stmt);
  return GetUpperStarOf(AFFECTS, ASSIGN, stmt);
}

IDENT_PAIR_VECTOR AffectsSession::GetAffectsPairs() {
  //return m_all_affects_pairs;
  return GetAllPairs();
  //return GetPairByType(ASSIGN, ASSIGN);
}

IDENT_PAIR_VECTOR AffectsSession::GetAffectsStarPairs() {
  //return m_all_affects_star_pairs;
  return GetAllStarPairs();
//  return GetPairByType(ASSIGN, ASSIGN);
}

IDENT_PAIR_VECTOR AffectsSession::GetAffectsSameSynPairs() {
  return m_same_affects_pairs;
}

IDENT_PAIR_VECTOR AffectsSession::GetAffectsStarSameSynPairs() {
  return m_same_affects_star_pairs;
}

// HELPER METHODS
std::string AffectsSession::GetFollowingOf(std::string &stmt_no) {
  return m_affects_store->GetFollowsStore()->GetFollowingOf(STMT, stmt_no);
}

std::unordered_set<std::string> AffectsSession::GetVarModByStmt(std::string &stmt_no) {
  return m_affects_store->GetModifyStore()->GetVarModByStmt(stmt_no);
}

std::unordered_set<std::string> AffectsSession::GetVarUsedByStmt(std::string &stmt_no) {
  return m_affects_store->GetUsageStore()->GetVarUsedByStmt(stmt_no);
}

void AffectsSession::TraverseCfg() {
  std::unordered_map<std::string, std::shared_ptr<source::CfgNode>>
      cfg_map = m_affects_store->GetProgramCfg()->GetCfgMap();
  for (auto const &cfg : cfg_map) {
    std::shared_ptr<source::CfgNode> proc_head = cfg.second;
    std::shared_ptr<source::CfgNode> tmp = std::make_shared<source::CfgNode>();
    std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map;


    m_terminating_node_stack.push(tmp);

    m_last_modified_map_stack.push(std::make_shared<std::unordered_map<std::string, std::unordered_set<std::string>>>(last_modified_map));
    TraverseCfg(proc_head);
    m_last_modified_map_stack.pop();

    m_terminating_node_stack.pop();
  }
}

void AffectsSession::TraverseCfg(std::shared_ptr<source::CfgNode> &cfg_node) {
  // checks on TraverseCfg from If-statement handler
  if (!cfg_node->GetStatementList().empty() && !m_terminating_node_stack.top()->GetStatementList().empty()) {
    if (cfg_node->GetStatementList().front().stmt_no == m_terminating_node_stack.top()->GetStatementList().front().stmt_no) {
      m_terminating_node_stack.pop();
      m_terminating_node_stack.push(cfg_node);
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
      HandleIfStatement(stmt_no, cfg_node);
      TraverseCfg(cfg_node);
      return;
    }
  }

  // check if there are no next nodes
  if (cfg_node->GetNextList().empty()) {
    return;
  }

  std::shared_ptr<source::CfgNode> next_node = cfg_node->GetNextList().back();
  TraverseCfg(next_node);
}

void AffectsSession::HandleAssignStatement(std::string stmt_no) {
  // check variables being used if they are modified
  // add them to affects map
  std::unordered_set<std::string> vars_used = GetVarUsedByStmt(stmt_no);
  for (auto const &var_used : vars_used) {
    // check if var_used is modified before
    if (m_last_modified_map_stack.top()->count(var_used) != 0) {
      std::unordered_set<std::string> last_mod_stmt_nos = m_last_modified_map_stack.top()->at(var_used);
      for (auto const &last_mod_stmt_no : last_mod_stmt_nos) {
//        // update affects (m_affects_map)
//        if (m_affects_map.count(last_mod_stmt_no) == 0) {
//          m_affects_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
//        }
//        m_affects_map.at(last_mod_stmt_no).insert(stmt_no);
//        // update affects (m_affects_reverse_map)
//        if (m_affects_reverse_map.count(stmt_no) == 0) {
//          m_affects_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
//        }
//        m_affects_reverse_map.at(stmt_no).insert(last_mod_stmt_no);
//        // update affects (m_all_affects_pairs)
//        m_all_affects_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
//        // update affects same synonym
        if (last_mod_stmt_no == stmt_no) {
          m_same_affects_pairs.push_back(std::make_pair(last_mod_stmt_no, stmt_no));
        }

        /////
        auto pair = std::make_pair(last_mod_stmt_no, stmt_no);
        if (m_all_affects_pairs2.find(pair) == m_all_affects_pairs2.end())
          AddAffects(false, ASSIGN, last_mod_stmt_no, ASSIGN, stmt_no);
        m_all_affects_pairs2.insert(pair);
        /////


        if (m_is_affects_star_involved) {
//          if (m_affects_star_map.count(last_mod_stmt_no) == 0) {
//            m_affects_star_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
//          }
//          m_affects_star_map.at(last_mod_stmt_no).insert(stmt_no);
//          // update affects star (m_affects_star_reverse_map)
//          if (m_affects_star_reverse_map.count(stmt_no) == 0) {
//            m_affects_star_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
//          }
//          m_affects_star_reverse_map.at(stmt_no).insert(last_mod_stmt_no);
//          // update affects star (m_all_affects_star_pairs)
//          m_all_affects_star_pairs.insert(std::make_pair(last_mod_stmt_no, stmt_no));
          if (last_mod_stmt_no == stmt_no) {
            m_same_affects_star_pairs.push_back(std::make_pair(last_mod_stmt_no, stmt_no));
          }

          /////
          auto pair = std::make_pair(last_mod_stmt_no, stmt_no);
          if (m_all_affects_star_pairs2.find(pair) == m_all_affects_star_pairs2.end())
            AddAffects(true, ASSIGN, last_mod_stmt_no, ASSIGN, stmt_no);
          m_all_affects_star_pairs2.insert(pair);
          /////

          if (m_last_modified_star_map.count(stmt_no) == 0) {
            m_last_modified_star_map.insert({stmt_no, std::unordered_set<std::string>()});
          }
          m_last_modified_star_map.at(stmt_no).insert(last_mod_stmt_no);

          for (const auto &p : m_last_modified_star_map[last_mod_stmt_no]) {
//            if (m_affects_star_map.count(p) == 0) {
//              m_affects_star_map.insert({p, std::unordered_set<std::string>()});
//            }
//            m_affects_star_map.at(p).insert(stmt_no);
//            // update affects star (m_affects_star_reverse_map)
//            if (m_affects_star_reverse_map.count(stmt_no) == 0) {
//              m_affects_star_reverse_map.insert({stmt_no, std::unordered_set<std::string>()});
//            }
//            m_affects_star_reverse_map.at(stmt_no).insert(p);
//            // update affects star (m_all_affects_star_pairs)
//            m_all_affects_star_pairs.insert(std::make_pair(p, stmt_no));
            if (p == stmt_no) {
              m_same_affects_star_pairs.push_back(std::make_pair(p, stmt_no));
            }

            /////
            auto pair = std::make_pair(p, stmt_no);
            if (m_all_affects_star_pairs2.find(pair) == m_all_affects_star_pairs2.end())
              AddAffects(true, ASSIGN, p, ASSIGN, stmt_no);
            m_all_affects_star_pairs2.insert(pair);
            /////

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
    if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_map_stack.top()->insert({var_mod, std::unordered_set<std::string>()});
    }
    m_last_modified_map_stack.top()->at(var_mod).clear();
    m_last_modified_map_stack.top()->at(var_mod).insert(stmt_no);
  }
}

void AffectsSession::HandleReadStatement(std::string stmt_no) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("READ SHOULD ONLY HAVE ONE VAR MOD");
  }

  std::string var_mod = *(vars_mod.begin());

  if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
    m_last_modified_map_stack.top()->insert({var_mod, std::unordered_set<std::string>()});
  }
  m_last_modified_map_stack.top()->at(var_mod).clear();
}

void AffectsSession::HandleCallStatement(std::string stmt_no) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (auto const &var_mod : vars_mod) {
    if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_map_stack.top()->insert({var_mod, std::unordered_set<std::string>()});
    }
    m_last_modified_map_stack.top()->at(var_mod).clear();
  }
}

void AffectsSession::HandleWhileStatement(std::shared_ptr<source::CfgNode> &cfg_node) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::shared_ptr<std::unordered_map<std::string, std::unordered_set<std::string>>> last_modified_map_clone = std::make_shared<std::unordered_map<std::string, std::unordered_set<std::string>>>(*(m_last_modified_map_stack.top()));
  std::shared_ptr<source::CfgNode> start_node = cfg_node->GetNextList().front();
  std::shared_ptr<source::CfgNode> end_node = cfg_node;

  // traverse twice to populate modified table properly
  m_last_modified_map_stack.push(last_modified_map_clone);

  m_terminating_node_stack.push(cfg_node);

  int affect_size_prev = 0;
  int affect_size_curr = 0;

  TraverseCfg(start_node);
//  TraverseCfg(start_node);
  if (m_is_affects_star_involved) {
    affect_size_prev = GetAffectsStarPairs().size();
  } else {
    affect_size_prev = GetAffectsPairs().size();
  }
  TraverseCfg(start_node);
  if (m_is_affects_star_involved) {
    affect_size_curr = GetAffectsStarPairs().size();
  } else {
    affect_size_curr = GetAffectsPairs().size();
  }

  while (affect_size_curr != affect_size_prev) {
    affect_size_prev = affect_size_curr;
    TraverseCfg(start_node);
    if (m_is_affects_star_involved) {
      affect_size_curr = GetAffectsStarPairs().size();
    } else {
      affect_size_curr = GetAffectsPairs().size();
    }
  }
  m_last_modified_map_stack.pop();
  m_terminating_node_stack.pop();

  // merge last_modified_map_clone into last_modified_map
  for (auto const &last_modified : *(last_modified_map_clone)) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (m_last_modified_map_stack.top()->count(var_name) == 0) {
      m_last_modified_map_stack.top()->insert({var_name, std::unordered_set<std::string>()});
    }
    m_last_modified_map_stack.top()->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }
}

void AffectsSession::HandleIfStatement(std::string stmt_no,
                                      std::shared_ptr<source::CfgNode> &cfg_node) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::shared_ptr<std::unordered_map<std::string, std::unordered_set<std::string>>> last_modified_map_clone = std::make_shared<std::unordered_map<std::string, std::unordered_set<std::string>>>(*(m_last_modified_map_stack.top()));

  std::shared_ptr<source::CfgNode> if_cfg_node = cfg_node->GetNextList()[0];
  std::shared_ptr<source::CfgNode> else_cfg_node = cfg_node->GetNextList()[1];
  std::shared_ptr<source::CfgNode> end_node = std::make_shared<source::CfgNode>();
  std::string end_node_stmt_no = GetFollowingOf(stmt_no);
  end_node->AddStatement(StmtType::STMT, end_node_stmt_no);

  // first TraverseIfCfg (if-block) will process statements and find dummy node
  // for the second TraverseIfCfg (else-block)
  TraverseCfg(if_cfg_node);

  m_last_modified_map_stack.push(last_modified_map_clone);
  TraverseCfg(else_cfg_node);
  m_last_modified_map_stack.pop();

  // merge last_modified_map_clone into last_modified_map
  for (auto const &last_modified : *(last_modified_map_clone)) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (m_last_modified_map_stack.top()->count(var_name) == 0) {
      m_last_modified_map_stack.top()->insert({var_name, std::unordered_set<std::string>()});
    }
    m_last_modified_map_stack.top()->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  cfg_node = end_node;
}
