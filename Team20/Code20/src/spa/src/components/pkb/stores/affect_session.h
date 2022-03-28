#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../../utils/pair_hash.h"

class ModifyStore;
class UsageStore;
class Cfg;
class CfgNode;

class AffectSession {
 private:
  std::shared_ptr<Cfg> m_program_cfg;
  std::shared_ptr<ModifyStore> m_modify_store;
  std::shared_ptr<UsageStore> m_usage_store;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_map;// <variable, stmt_no>

 public:
  explicit AffectSession(std::shared_ptr<Cfg> program_cfg,
                         std::shared_ptr<ModifyStore> modify_store,
                         std::shared_ptr<UsageStore> usage_store);
  std::unordered_set<std::string> GetVarModByStmt(std::string stmt_no);
  std::unordered_set<std::string> GetVarUsedByStmt(std::string stmt_no);
  void TraverseCfg();
  void TraverseCfg(std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &cfg_terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleAssignStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleReadStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleCallStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleWhileStatement(std::string stmt_no, std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &cfg_terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleIfStatement(std::string stmt_no, std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &cfg_terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_

// Notes:

// Affects(_, _)

// Affects(a, _)
// Affects(_, a)
// Affects(a1, a2)

// Affects(s, _)
// Affects(_, s)
// Affects(s1, s2)

// Affects(a, s)
// Affects(s, a)

// Affects(1, _)
// Affects(_, 1)
// Affects(1, 2)

// Affects(1, s)
// Affects(s, 1)
// Affects(1, a)
// Affects(a, 1)
