#include "affects_store.h"
#include "components/pkb/stores/modifies_store/modifies_store.h"
#include "components/pkb/stores/uses_store/uses_store.h"
#include "components/pkb/stores/affects_store/affect_store_factory.h"
#include "components/source_subsystem/types/cfg/cfg.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

namespace pkb {

AffectsStore::AffectsStore(IDENT_SET_VECTOR_PTR stmt_vector,
                           IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                           AffectsStoreFactoryPtr affects_store_factory,
                           bool is_affects_star_involved) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)),
    m_affects_store_factory(std::move(affects_store_factory)),
    m_is_affects_star_involved(is_affects_star_involved),
    m_last_modified_star_map(IDENT_SET_MAP()),
    m_terminating_node_stack(std::stack<source::CfgNodePtr>()) {
  HandleCfg();
}

bool AffectsStore::IsAffected(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_pairs.begin(), m_all_affects_pairs.end(), predicate);
}

bool AffectsStore::IsAffectedStar(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.second == stmt;
  };
  return std::any_of(m_all_affects_star_pairs.begin(), m_all_affects_star_pairs.end(), predicate);
}

bool AffectsStore::IsAffecting(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_pairs.begin(), m_all_affects_pairs.end(), predicate);
}

bool AffectsStore::IsAffectingStar(IDENT const &stmt) {
  auto predicate = [stmt](const IDENT_PAIR &pair) {
    return pair.first == stmt;
  };
  return std::any_of(m_all_affects_star_pairs.begin(), m_all_affects_star_pairs.end(), predicate);
}

bool AffectsStore::DoesAffectExists(IDENT_PAIR const &pair) {
  if (m_all_affects_pairs.find(pair) != m_all_affects_pairs.end()) {
    return true;
  }
  return false;
}

bool AffectsStore::DoesAffectStarExists(IDENT_PAIR const &pair) {
  if (m_all_affects_star_pairs.find(pair) != m_all_affects_star_pairs.end()) {
    return true;
  }
  return false;
}

IDENT_SET AffectsStore::GetAffectedOf(IDENT const &stmt) {
  return GetLowerSetOf(ASSIGN, stmt);
}

IDENT_SET AffectsStore::GetAffectedStarOf(IDENT const &stmt) {
  return GetLowerStarOf(ASSIGN, stmt);
}

IDENT_SET AffectsStore::GetAffectsOf(IDENT const &stmt) {
  return GetUpperSetOf(AFFECTS, ASSIGN, stmt);
}

IDENT_SET AffectsStore::GetAffectsStarOf(IDENT const &stmt) {
  return GetUpperStarOf(ASSIGN, stmt);
}

IDENT_PAIR_VECTOR AffectsStore::GetAffectsPairs() {
  return GetPairByType(STMT, STMT);
}

IDENT_PAIR_VECTOR AffectsStore::GetAffectsStarPairs() {
  return GetStarPairByType(STMT, STMT);
}

IDENT_SET AffectsStore::GetAffectsSameSynSet() {
  return m_same_affects_set;
}

IDENT_SET AffectsStore::GetAffectsStarSameSynSet() {
  return m_same_affects_star_set;
}

IDENT_SET AffectsStore::GetVarModByStmt(IDENT &stmt_no) {
  return m_affects_store_factory->GetModifyStore()->GetVarModByStmt(stmt_no);
}

IDENT_SET AffectsStore::GetVarUsedByStmt(IDENT &stmt_no) {
  return m_affects_store_factory->GetUsesStore()->GetVarUsedByStmt(stmt_no);
}

unsigned int AffectsStore::GetMaxPairSize() {
  if (m_is_affects_star_involved) {
    return GetAffectsStarPairs().size();
  }
  return GetAffectsPairs().size();
}

void AffectsStore::InsertToIdentSetMap(IDENT_SET_MAP &map, IDENT &key, IDENT &value) {
  if (map.count(key) == 0) {
    map.insert({key, IDENT_SET()});
  }
  map.at(key).insert(value);
}

void AffectsStore::MergeModifiedTable(IDENT_SET_MAP_PTR &table1, IDENT_SET_MAP_PTR &table2) {
  // merge table2 into table1
  for (auto const &last_modified : *(table2)) {
    IDENT var_name = last_modified.first;
    IDENT_SET stmt_nos = last_modified.second;
    if (table1->count(var_name) == 0) {
      table1->insert({var_name, IDENT_SET()});
    }
    table1->at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }
}

void AffectsStore::UpdateModifiedTable(IDENT &stmt_no, bool is_clear_only) {
  IDENT_SET vars_mod = GetVarModByStmt(stmt_no);
  for (auto const &var_mod : vars_mod) {
    if (m_last_modified_map_stack.top()->count(var_mod) == 0) {
      m_last_modified_map_stack.top()->insert({var_mod, IDENT_SET()});
    }
    m_last_modified_map_stack.top()->at(var_mod).clear();

    if (!is_clear_only) {
      m_last_modified_map_stack.top()->at(var_mod).insert(stmt_no);
    }
  }
}

void AffectsStore::HandleAffectsLastModSet(IDENT &var_used, IDENT &stmt_no, IDENT_SET_MAP_PTR &last_modified_map) {
  IDENT_SET last_mod_set = last_modified_map->at(var_used);
  for (IDENT last_mod : last_mod_set) {
    AddAffects(false, last_mod, stmt_no);
    HandleAffectsStarLastModSet(last_mod, stmt_no);
  }
}

void AffectsStore::HandleAffectsStarLastModSet(IDENT &last_mod_stmt_no, IDENT &stmt_no) {
  if (m_is_affects_star_involved) {
    AddAffects(true, last_mod_stmt_no, stmt_no);
    InsertToIdentSetMap(m_last_modified_star_map, stmt_no, last_mod_stmt_no);
    HandleAffectsStarLastModStarSet(last_mod_stmt_no, stmt_no);
  }
}

void AffectsStore::HandleAffectsStarLastModStarSet(IDENT &last_mod_stmt_no, IDENT &stmt_no) {
  if (m_last_modified_star_map.count(last_mod_stmt_no) == 0) {
    m_last_modified_star_map.insert({last_mod_stmt_no, IDENT_SET()});
  }

  IDENT_SET last_mod_star_set = m_last_modified_star_map.at(last_mod_stmt_no);
  for (IDENT last_mod_star : last_mod_star_set) {
    AddAffects(true, last_mod_star, stmt_no);
    InsertToIdentSetMap(m_last_modified_star_map, stmt_no, last_mod_star);
  }
}

void AffectsStore::AddAffects(bool is_star, IDENT &upper, IDENT &lower) {
  IDENT_PAIR_SET *pair_set;
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
  if (pair_set->find(pair) == pair_set->end()) {
    //StmtStmtStore::AddAffects(is_star, ASSIGN, upper, ASSIGN, lower);
    if (!is_star) {
      ExhaustiveAddAllStmt(ASSIGN, upper, ASSIGN, lower, false);
      //ExhaustiveAddSubStmt(type1, upper, type2, lower, &type_pair_map);

    } else {
      ExhaustiveAddAllStmt(ASSIGN, upper, ASSIGN, lower, true);
      //ExhaustiveAddSubStmt(type1, upper, type2, lower, &star_type_pair_map);
    }
  }
  pair_set->insert(pair);
}

void AffectsStore::HandleCfg() {
  auto const &cfg_map = m_affects_store_factory->GetProgramCfg()->GetCfgMap();
  for (auto const &cfg : cfg_map) {
    source::CfgNodePtr proc_head = cfg.second;
    source::CfgNodePtr proc_tail = std::make_shared<source::CfgNode>();

    m_terminating_node_stack.push(proc_tail);
    IDENT_SET_MAP last_modified_map;
    m_last_modified_map_stack.push(std::make_shared<IDENT_SET_MAP>(last_modified_map));
    HandleCfg(proc_head);
    m_last_modified_map_stack.pop();
    m_terminating_node_stack.pop();
  }
}

void AffectsStore::HandleCfg(source::CfgNodePtr &cfg_node) {
  // end current recursive call if reaches terminating node
  if (!cfg_node->GetStatementList().empty() && !m_terminating_node_stack.top()->GetStatementList().empty()) {
    if (cfg_node->GetStatementList().front().stmt_no
        == m_terminating_node_stack.top()->GetStatementList().front().stmt_no) {
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
    } else if (type == READ || type == CALL) {
      HandleModifiableStatement(stmt_no);
    } else if (type == WHILE) {
      HandleWhileStatement(cfg_node);
    } else if (type == IF) {
      HandleIfStatement(cfg_node);
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

void AffectsStore::HandleAssignStatement(IDENT &stmt_no) {
  IDENT_SET_MAP_PTR last_modified_map = m_last_modified_map_stack.top();
  IDENT_SET vars_used = GetVarUsedByStmt(stmt_no);
  for (IDENT var_used : vars_used) {
    // check if var_used is modified before
    if (last_modified_map->count(var_used) != 0) {
      HandleAffectsLastModSet(var_used, stmt_no, last_modified_map);
    }
  }
  UpdateModifiedTable(stmt_no, false);
}

void AffectsStore::HandleModifiableStatement(IDENT &stmt_no) {
  // since, read/call is not an assign stmt, we can clear it from the table
  UpdateModifiedTable(stmt_no, true);
}

void AffectsStore::HandleWhileStatement(source::CfgNodePtr &cfg_node) {
  IDENT_SET_MAP_PTR last_modified_map = m_last_modified_map_stack.top();
  IDENT_SET_MAP_PTR last_modified_map_clone = std::make_shared<IDENT_SET_MAP>(*(last_modified_map));
  source::CfgNodePtr first_child_node = cfg_node->GetNextList().front();

  m_last_modified_map_stack.push(last_modified_map_clone);
  m_terminating_node_stack.push(cfg_node);

  // may need more > 1 traversal around the
  // while-statement scope to properly populate
  // affects/affects* pair
  unsigned int affect_size_prev = GetMaxPairSize();
  HandleCfg(first_child_node);
  unsigned int affect_size_curr = GetMaxPairSize();
  while (affect_size_curr != affect_size_prev) {
    affect_size_prev = affect_size_curr;
    HandleCfg(first_child_node);
    affect_size_curr = GetMaxPairSize();
  }

  m_last_modified_map_stack.pop();
  m_terminating_node_stack.pop();

  // merge last_modified_map_clone into last_modified_map
  MergeModifiedTable(last_modified_map, last_modified_map_clone);
}

void AffectsStore::HandleIfStatement(source::CfgNodePtr &cfg_node) {
  IDENT_SET_MAP_PTR last_modified_map = m_last_modified_map_stack.top();
  IDENT_SET_MAP_PTR last_modified_map_clone = std::make_shared<IDENT_SET_MAP>(*(last_modified_map));
  source::CfgNodePtr if_cfg_node = cfg_node->GetNextList()[0];
  source::CfgNodePtr else_cfg_node = cfg_node->GetNextList()[1];
  source::CfgNodePtr end_node = std::make_shared<source::CfgNode>();

  HandleCfg(if_cfg_node);
  m_last_modified_map_stack.push(last_modified_map_clone);
  HandleCfg(else_cfg_node);
  m_last_modified_map_stack.pop();

  MergeModifiedTable(last_modified_map, last_modified_map_clone);
  cfg_node = end_node;
}

}
