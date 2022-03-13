#ifndef PKB_H
#define PKB_H

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "stores/usage_store.h"
#include "stores/modify_store.h"
#include "stores/follow_store.h"
#include "stores/parent_store.h"
#include "expression_tree.h"
#include "components/source_subsystem/cfg/cfg.h"

class PKB {
 private:
  std::shared_ptr<FollowStore> m_follow_store;
  std::shared_ptr<ParentStore> m_parent_store;
  std::shared_ptr<UsageStore> m_usage_store;
  std::shared_ptr<ModifyStore> m_modify_store;
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;
  std::unordered_map<std::string, std::pair<std::string, std::string>> m_pattern_map;
  std::shared_ptr<source::ProgramCfg> m_program_cfg;

 public:
  PKB();
  void InitRelationshipStores();
  void AddStmt(std::string const &name, StmtType type);
  void AddPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs);
  void AddProgramCfg(std::shared_ptr<source::ProgramCfg> program_cfg);
  [[nodiscard]] std::unordered_set<std::string> GetStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::string> GetStmtWithPattern(std::string const &lhs, std::string rhs);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>,
                                   pair_hash> GetStmtWithPatternSynonym(std::string rhs);
  [[nodiscard]] std::shared_ptr<FollowStore> GetFollowStore();
  [[nodiscard]] std::shared_ptr<ModifyStore> GetModifyStore();
  [[nodiscard]] std::shared_ptr<ParentStore> GetParentStore();
  [[nodiscard]] std::shared_ptr<UsageStore> GetUsageStore();
};

#endif //PKB_H
