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

    std::unordered_map<std::string, std::string> last_modified_map;
    TraverseCfg(cfg_head, last_modified_map);
  }
}

void AffectSession::TraverseCfg(std::shared_ptr<CfgNode> cfg_node, std::unordered_map<std::string, std::string> &last_modified_map) {
  std::vector<Statement> statements = cfg_node->GetStatementList();

  // check if dummy node, but not last node
  if (statements.size() == 0 && cfg_node->GetDescendants().size() > 0) {
    std::shared_ptr<CfgNode> next_node = cfg_node->GetDescendants().front();
    TraverseCfg(next_node, last_modified_map);
  }

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
      HandleWhileStatement(stmt_no, last_modified_map);
    } else if (type == IF) {
      HandleIfStatement(stmt_no, last_modified_map);
    }
  }
}

void AffectSession::HandleAssignStatement(std::string stmt_no, std::unordered_map<std::string, std::string> &last_modified_map) {
  // check variables being used if they are modified
  // add them to affects map
  std::unordered_set<std::string> vars_used = GetVarUsedByStmt(stmt_no);
  for (std::string var_used : vars_used) {
    // check if var_used is modified before
    if (last_modified_map.count(var_used) != 0) {
      std::string last_mod_stmt_no = last_modified_map.at(var_used);

      // create set if it does not exist
      if (m_affects_map.count(last_mod_stmt_no) == 0) {
        m_affects_map.at(last_mod_stmt_no) = std::unordered_set<std::string>();
      }

      // update affects
      m_affects_map.at(last_mod_stmt_no).insert(stmt_no);
    }
  }

  // update modified table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("ASSIGN SHOULD ONLY HAVE ONE VAR MOD");
  }

  std::string var_mod = *(vars_mod.begin());
  last_modified_map.at(var_mod) = stmt_no;
}

void AffectSession::HandleReadStatement(std::string stmt_no, std::unordered_map<std::string, std::string> &last_modified_map) {
  // update modified table
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  if (vars_mod.size() != 1) {
    // TODO: replace with custom exception
    throw std::runtime_error("READ SHOULD ONLY HAVE ONE VAR MOD");
  }

  std::string var_mod = *(vars_mod.begin());
  last_modified_map.at(var_mod) = stmt_no;
}

void AffectSession::HandleCallStatement(std::string stmt_no, std::unordered_map<std::string, std::string> &last_modified_map) {
  // update modified table
  // call statements can have multiple modified vars
  std::unordered_set<std::string> vars_mod = GetVarModByStmt(stmt_no);
  for (std::string var_mod : vars_mod) {
    last_modified_map.at(var_mod) = stmt_no;
  }
}

void AffectSession::HandleWhileStatement(std::string stmt_no, std::unordered_map<std::string, std::string> &last_modified_map) {

}

void AffectSession::HandleIfStatement(std::string stmt_no, std::unordered_map<std::string, std::string> &last_modified_map) {

}
