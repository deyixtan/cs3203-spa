#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_STORES_AFFECT_SESSION_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../../../utils/pair_hash.h"
#include "../../pkb.h"

class AffectStore;
class Cfg;
class CfgNode;

class AffectSession {
 private:
  std::shared_ptr<AffectStore> m_affects_store;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_reverse_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_all_affects_pairs;

 private:
  std::unordered_set<std::string> GetVarModByStmt(std::string stmt_no);
  std::unordered_set<std::string> GetVarUsedByStmt(std::string stmt_no);
  void TraverseCfg();
  void TraverseCfg(std::shared_ptr<CfgNode> &cfg_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void TraverseWhileCfg(std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void TraverseIfCfg(std::shared_ptr<CfgNode> &cfg_node, std::shared_ptr<CfgNode> &terminating_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleAssignStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleReadStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleCallStatement(std::string stmt_no, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleWhileStatement(std::shared_ptr<CfgNode> &cfg_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);
  void HandleIfStatement(std::shared_ptr<CfgNode> &cfg_node, std::unordered_map<std::string, std::unordered_set<std::string>> &last_modified_map);

 public:
  explicit AffectSession(std::shared_ptr<AffectStore> affects_store);
  [[nodiscard]] bool IsAffected(std::string const &stmt);
  [[nodiscard]] bool IsAffecting(std::string const &stmt);
  [[nodiscard]] bool DoesAffectExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetAffectedOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAffectsOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAffectsPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllAffectsStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllAffectsStmt(StmtType type1,
                                                                                                     StmtType type2);
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
