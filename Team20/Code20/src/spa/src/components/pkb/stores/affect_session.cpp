#include "affect_session.h"
#include "modify_store.h"
#include "usage_store.h"
#include "../../source_subsystem/types/cfg/cfg.h"
#include "../../source_subsystem/types/cfg/cfg_node.h"

AffectSession::AffectSession(std::shared_ptr<Cfg> program_cfg,
                             std::shared_ptr<ModifyStore> modify_store,
                             std::shared_ptr<UsageStore> usage_store)
    : m_program_cfg(program_cfg), m_modify_store(modify_store), m_usage_store(usage_store) {
  TraverseCfg();
}

// HELPER METHODS
std::unordered_set<std::string> AffectSession::GetVarModByStmt(std::string stmt_no) {
  return m_modify_store->GetVarModByStmt(stmt_no);
}

std::unordered_set<std::string> AffectSession::GetVarUsedByStmt(std::string stmt_no) {
  return m_usage_store->GetVarUsedByStmt(stmt_no);
}

// POPULATE METHODS
void AffectSession::TraverseCfg() {
  for (auto const &cfg : m_program_cfg->GetCfgMap()) {
    std::shared_ptr<CfgNode> cfg_head = cfg.second;

    std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map;
    std::shared_ptr<CfgNode> tmp = std::shared_ptr<CfgNode>();
    TraverseCfg(cfg_head, tmp, last_modified_map);
  }
}

void AffectSession::TraverseCfg(std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &cfg_terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map) {
  std::vector<Statement> statements = cfg_node->GetStatementList();

  // check dummy node statement, skip
  if (statements.size() == 0) {
    cfg_terminating_node = cfg_node;
    return;
  }

  // is terminating
  if (cfg_node == cfg_terminating_node) {
    return;
  }

  // process statements in a cfg_node
  for (auto const &stmt : statements) {
    std::string stmt_no = stmt.stmt_no;
    StmtType type = stmt.type;
    if (type == ASSIGN) {
      HandleAssignStatement(stmt_no, last_modified_map);
    } else if (type == READ) {
      HandleReadStatement(stmt_no, last_modified_map);
    } else if (type == CALL) {
      HandleCallStatement(stmt_no, last_modified_map);
    } else if (type == WHILE) {
      HandleWhileStatement(stmt_no, cfg_node, cfg_node, last_modified_map);
    } else if (type == IF) {
      HandleIfStatement(stmt_no, cfg_node, cfg_terminating_node, last_modified_map);
    }
  }

  // process next cfg_node
  if (cfg_node->GetDescendants().size() > 0) {
    std::shared_ptr<CfgNode> next_node = cfg_node->GetDescendants().back();
    TraverseCfg(next_node, cfg_terminating_node, last_modified_map);
  }
}

void AffectSession::HandleAssignStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map) {
  // check variables being used if they are modified
  // add them to affects map
  std::unordered_set<std::string> vars_used = GetVarUsedByStmt(stmt_no);
  for (std::string var_used : vars_used) {
    // check if var_used is modified before
    if (last_modified_map.count(var_used) != 0) {
      std::unordered_set<std::string> last_mod_stmt_nos = last_modified_map.at(var_used);
      for (auto const last_mod_stmt_no : last_mod_stmt_nos) {
        // update affects
        if (m_affects_map.count(last_mod_stmt_no) == 0) {
          m_affects_map.insert({last_mod_stmt_no, std::unordered_set<std::string>()});
        }
        m_affects_map.at(last_mod_stmt_no).insert(stmt_no);
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

void AffectSession::HandleReadStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map) {
  // update modified table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("READ SHOULD ONLY HAVE ONE VAR MOD");
  }

  std::string var_mod = *(vars_mod.begin());
  last_modified_map.at(var_mod).clear();
}

void AffectSession::HandleCallStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map) {
  // update modified table
  // call statements can have multiple modified vars
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (std::string var_mod : vars_mod) {
    last_modified_map.at(var_mod).clear();
  }
}

void AffectSession::HandleWhileStatement(std::string stmt_no, std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &cfg_terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map) {
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map_clone = last_modified_map;
  std::shared_ptr<CfgNode> tmp1 = cfg_node->GetDescendants().front();
  std::shared_ptr<CfgNode> tmp2 = cfg_terminating_node;
  std::shared_ptr<CfgNode> tmp3 = cfg_node->GetDescendants().front(); // save state
  std::shared_ptr<CfgNode> tmp4 = cfg_terminating_node; // save state
  TraverseCfg(tmp1, tmp2, last_modified_map_clone);
  TraverseCfg(tmp3, tmp4, last_modified_map_clone);
}

void AffectSession::HandleIfStatement(std::string stmt_no, std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &cfg_terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map) {
  std::shared_ptr<CfgNode> if_cfg_node = cfg_node->GetDescendants()[0];
  std::shared_ptr<CfgNode> else_cfg_node = cfg_node->GetDescendants()[1];
  // create a copy of last_modified_map, to be served to the "else" cfg node
  std::unordered_map<std::string, std::unordered_set<std::string>> last_modified_map_clone = last_modified_map;

  TraverseCfg(if_cfg_node, cfg_terminating_node, last_modified_map);
  TraverseCfg(else_cfg_node, cfg_terminating_node, last_modified_map_clone);

  // merge last_modified_map_clone into last_modified_map
  for (auto last_modified : last_modified_map_clone) {
    std::string var_name = last_modified.first;
    std::unordered_set<std::string> stmt_nos = last_modified.second;
    if (last_modified_map.count(var_name) == 0) {
      last_modified_map.insert({var_name, std::unordered_set<std::string>()});
    }
    last_modified_map.at(var_name).insert(stmt_nos.begin(), stmt_nos.end());
  }

  cfg_node = cfg_terminating_node;
}
