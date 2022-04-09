#ifndef AFFECT_SESSION_H
#define AFFECT_SESSION_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

#include "components/source_subsystem/source_declarations.h"

#include "../../../../utils/pair_hash.h"
#include "../../pkb.h"

class AffectStore;

class AffectSession {
 private:
  std::shared_ptr<AffectStore> m_affects_store;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_star_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_reverse_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_affects_star_reverse_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_all_affects_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_all_affects_star_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_same_affects_pairs; // for same synonym
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_same_affects_star_pairs; // for same synonym
  std::stack<std::shared_ptr<std::unordered_map<std::string, std::unordered_set<std::string>>>> m_last_modified_map_stack;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_last_modified_star_map;
  bool m_is_affect_star_involved;

 private:
  std::unordered_set<std::string> GetVarModByStmt(std::string &stmt_no);
  std::unordered_set<std::string> GetVarUsedByStmt(std::string &stmt_no);
  std::string GetFollowingOf(std::string &stmt_no);
  void TraverseCfg();
  void TraverseCfg(std::shared_ptr<source::CfgNode> &cfg_node, std::shared_ptr<source::CfgNode> &terminating_node);
  void HandleAssignStatement(std::string stmt_no);
  void HandleReadStatement(std::string stmt_no);
  void HandleCallStatement(std::string stmt_no);
  void HandleWhileStatement(std::shared_ptr<source::CfgNode> &cfg_node);
  void HandleIfStatement(std::string stmt_no, std::shared_ptr<source::CfgNode> &cfg_node, std::shared_ptr<source::CfgNode> &terminating_node);

 public:
  explicit AffectSession(std::shared_ptr<AffectStore> affects_store, bool is_affect_star_involved);
  [[nodiscard]] bool IsAffected(std::string const &stmt);
  [[nodiscard]] bool IsAffectedStar(std::string const &stmt);
  [[nodiscard]] bool IsAffecting(std::string const &stmt);
  [[nodiscard]] bool IsAffectingStar(std::string const &stmt);
  [[nodiscard]] bool DoesAffectExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool DoesAffectStarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetAffectedOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAffectedStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAffectsOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAffectsStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAffectsPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAffectsStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAffectsSameSynPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAffectsStarSameSynPairs();
};

#endif //AFFECT_SESSION_H
