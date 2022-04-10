#include "affects_session.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

AffectsSession::AffectsSession(IDENT_SET_VECTOR_PTR stmt_vector,
                               IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                               AffectsStorePtr affects_store,
                               bool is_affects_star_involved) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)),
    m_affects_store(std::move(affects_store)),
    m_last_modified_star_map(IDENT_SET_MAP()),
    m_is_affects_star_involved(is_affects_star_involved),
    m_terminating_node_stack(std::stack<source::CfgNodePtr>()) {
  HandleCfg();
}

bool AffectsSession::IsAffected(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_pairs.begin(), m_all_affects_pairs.end(), predicate);
}

bool AffectsSession::IsAffectedStar(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_star_pairs.begin(), m_all_affects_star_pairs.end(), predicate);
}

bool AffectsSession::IsAffecting(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_pairs.begin(), m_all_affects_pairs.end(), predicate);
}

bool AffectsSession::IsAffectingStar(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_star_pairs.begin(), m_all_affects_star_pairs.end(), predicate);
}

bool AffectsSession::DoesAffectExists(IDENT_PAIR const &pair) {
  if (m_all_affects_pairs.find(pair) != m_all_affects_pairs.end()) {
    return true;
  }
  return false;
}

bool AffectsSession::DoesAffectStarExists(IDENT_PAIR const &pair) {
  if (m_all_affects_star_pairs.find(pair) != m_all_affects_star_pairs.end()) {
    return true;
  }
  return false;
}

IDENT_SET AffectsSession::GetAffectedOf(IDENT const &stmt) {
  return GetLowerSetOf(AFFECTS, ASSIGN, stmt);
}

IDENT_SET AffectsSession::GetAffectedStarOf(IDENT const &stmt) {
  return GetLowerStarOf(AFFECTS, ASSIGN, stmt);
}

IDENT_SET AffectsSession::GetAffectsOf(IDENT const &stmt) {
  return GetUpperSetOf(AFFECTS, ASSIGN, stmt);
}

IDENT_SET AffectsSession::GetAffectsStarOf(IDENT const &stmt) {
  return GetUpperStarOf(AFFECTS, ASSIGN, stmt);
}

IDENT_PAIR_VECTOR AffectsSession::GetAffectsPairs() {
  return GetAllPairs();
}

IDENT_PAIR_VECTOR AffectsSession::GetAffectsStarPairs() {
  return GetAllStarPairs();
}

IDENT_SET AffectsSession::GetAffectsSameSynSet() {
  return m_same_affects_set;
}

IDENT_SET AffectsSession::GetAffectsStarSameSynSet() {
  return m_same_affects_star_set;
}

// HELPER METHODS
IDENT AffectsSession::GetFollowingOf(IDENT &stmt_no) {
  return m_affects_store->GetFollowsStore()->GetFollowingOf(STMT, stmt_no);
}

IDENT_SET AffectsSession::GetVarModByStmt(IDENT &stmt_no) {
  return m_affects_store->GetModifyStore()->GetVarModByStmt(stmt_no);
}

IDENT_SET AffectsSession::GetVarUsedByStmt(IDENT &stmt_no) {
  return m_affects_store->GetUsageStore()->GetVarUsedByStmt(stmt_no);
}

void AffectsSession::AddAffects(bool is_star, IDENT upper, IDENT lower) {
  IDENT_PAIR_SET* pair_set;
  if (is_star) {
    pair_set = &m_all_affects_star_pairs;
  } else {
    pair_set = &m_all_affects_pairs;
  }

  // handle same synonym
  if (upper == lower) {
    if (is_star) {
      m_same_affects_star_set.insert(upper);
    } else {
      m_same_affects_set.insert(upper);
    }
  }

  // check for duplication before exhaustive add
  auto pair = std::make_pair(upper, lower);
  if (pair_set->find(pair) == pair_set->end())
    StmtStmtStore::AddAffects(is_star, ASSIGN, upper, ASSIGN, lower);
  pair_set->insert(pair);
}

void AffectsSession::HandleCfg() {
  auto const &cfg_map = m_affects_store->GetProgramCfg()->GetCfgMap();
  for (auto const &cfg : cfg_map) {
    source::CfgNodePtr proc_head = cfg.second;
    source::CfgNodePtr tmp = std::make_shared<source::CfgNode>();
    IDENT_SET_MAP last_modified_map;


    m_terminating_node_stack.push(tmp);

    m_last_modified_map_stack.push(std::make_shared<IDENT_SET_MAP>(last_modified_map));
    HandleCfg(proc_head);
    m_last_modified_map_stack.pop();

    m_terminating_node_stack.pop();
  }
}

void AffectsSession::HandleCfg(source::CfgNodePtr &cfg_node) {
  // checks on HandleCfg from If-statement handler
  if (!cfg_node->GetStatementList().empty() && !m_terminating_node_stack.top()->GetStatementList().empty()) {
    if (cfg_node->GetStatementList().front().stmt_no == m_terminating_node_stack.top()->GetStatementList().front().stmt_no) {
      m_terminating_node_stack.pop();
      m_terminating_node_stack.push(cfg_node);
      return;
    }
  }

  // process statements in a cfg_node
  auto const &statements = cfg_node->GetStatementList();
  for (auto const &stmt : statements) {
    IDENT stmt_no = stmt.stmt_no;
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
      HandleCfg(cfg_node);
      return;
    }
  }

  // check if there are no next nodes
  if (cfg_node->GetNextList().empty()) {
    return;
  }

  source::CfgNodePtr next_node = cfg_node->GetNextList().back();
  HandleCfg(next_node);
}

void AffectsSession::HandleAssignStatement(IDENT stmt_no) {
  // check variables being used if they are modified
  // add them to affects map
  IDENT_SET vars_used = GetVarUsedByStmt(stmt_no);
  for (auto const &var_used : vars_used) {
    // check if var_used is modified before
    if (m_last_modified_map_stack.top()->count(var_used) != 0) {
      IDENT_SET last_mod_stmt_nos = m_last_modified_map_stack.top()->at(var_used);
      for (auto const &last_mod_stmt_no : last_mod_stmt_nos) {
        AddAffects(false, last_mod_stmt_no, stmt_no);


        if (m_is_affects_star_involved) {
          AddAffects(true, last_mod_stmt_no, stmt_no);

          if (m_last_modified_star_map.count(stmt_no) == 0) {
            m_last_modified_star_map.insert({stmt_no, IDENT_SET()});
          }
          m_last_modified_star_map.at(stmt_no).insert(last_mod_stmt_no);

          for (const auto &p : m_last_modified_star_map[last_mod_stmt_no]) {
            AddAffects(true, p, stmt_no);

            if (m_last_modified_star_map.count(stmt_no) == 0) {
              m_last_modified_star_map.insert({stmt_no, IDENT_SET()});
            }
            m_last_modified_star_map.at(stmt_no).insert(p);
          }
        }
      }
    }
  }

  // update modified table
  IDENT_SET vars_mod = GetVarModByStmt(stmt_no);
  for (auto const &var_mod : vars_mod) {
    if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_map_stack.top()->insert({var_mod, IDENT_SET()});
    }
    m_last_modified_map_stack.top()->at(var_mod).clear();
    m_last_modified_map_stack.top()->at(var_mod).insert(stmt_no);
  }
}

void AffectsSession::HandleReadStatement(IDENT stmt_no) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  IDENT_SET vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("READ SHOULD ONLY HAVE ONE VAR MOD");
  }

  IDENT var_mod = *(vars_mod.begin());

  if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
    m_last_modified_map_stack.top()->insert({var_mod, IDENT_SET()});
  }
  m_last_modified_map_stack.top()->at(var_mod).clear();
}

void AffectsSession::HandleCallStatement(IDENT stmt_no) {
  // update modified table
  // since, read is not an assign stmt, we cleared it from the table
  IDENT_SET vars_mod = GetVarModByStmt(stmt_no);
  for (auto const &var_mod : vars_mod) {
    if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_map_stack.top()->insert({var_mod, IDENT_SET()});
    }
    m_last_modified_map_stack.top()->at(var_mod).clear();
  }
}

void AffectsSession::HandleWhileStatement(source::CfgNodePtr &cfg_node) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  IDENT_SET_MAP_PTR last_modified_map_clone = std::make_shared<IDENT_SET_MAP>(*(m_last_modified_map_stack.top()));
  source::CfgNodePtr start_node = cfg_node->GetNextList().front();
  source::CfgNodePtr end_node = cfg_node;

  // traverse twice to populate modified table properly
  m_last_modified_map_stack.push(last_modified_map_clone);

  m_terminating_node_stack.push(cfg_node);

  int affect_size_prev = 0;
  int affect_size_curr = 0;

  HandleCfg(start_node);
//  HandleCfg(start_node);
  if (m_is_affects_star_involved) {
    affect_size_prev = GetAffectsStarPairs().size();
  } else {
    affect_size_prev = GetAffectsPairs().size();
  }
  HandleCfg(start_node);
  if (m_is_affects_star_involved) {
    affect_size_curr = GetAffectsStarPairs().size();
  } else {
    affect_size_curr = GetAffectsPairs().size();
  }

  while (affect_size_curr != affect_size_prev) {
    affect_size_prev = affect_size_curr;
    HandleCfg(start_node);
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
    IDENT var_name = last_modified.first;
    IDENT_SET stmt_nos = last_modified.second;
    if (m_last_modified_map_stack.top()->count(var_name) == 0) {
      m_last_modified_map_stack.top()->insert({var_name, IDENT_SET()});
    }
    m_last_modified_map_stack.top()->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }
}

void AffectsSession::HandleIfStatement(IDENT stmt_no,
                                      source::CfgNodePtr &cfg_node) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  IDENT_SET_MAP_PTR last_modified_map_clone = std::make_shared<IDENT_SET_MAP>(*(m_last_modified_map_stack.top()));

  source::CfgNodePtr if_cfg_node = cfg_node->GetNextList()[0];
  source::CfgNodePtr else_cfg_node = cfg_node->GetNextList()[1];
  source::CfgNodePtr end_node = std::make_shared<source::CfgNode>();
  IDENT end_node_stmt_no = GetFollowingOf(stmt_no);
  end_node->AddStatement(STMT, end_node_stmt_no);

  // first TraverseIfCfg (if-block) will process statements and find dummy node
  // for the second TraverseIfCfg (else-block)
  HandleCfg(if_cfg_node);

  m_last_modified_map_stack.push(last_modified_map_clone);
  HandleCfg(else_cfg_node);
  m_last_modified_map_stack.pop();

  // merge last_modified_map_clone into last_modified_map
  for (auto const &last_modified : *(last_modified_map_clone)) {
    IDENT var_name = last_modified.first;
    IDENT_SET stmt_nos = last_modified.second;
    if (m_last_modified_map_stack.top()->count(var_name) == 0) {
      m_last_modified_map_stack.top()->insert({var_name, IDENT_SET()});
    }
    m_last_modified_map_stack.top()->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  cfg_node = end_node;
}
