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
#include "stores/call_store.h"
#include "stores/pattern_store.h"
#include "expression_tree.h"
#include "../source_subsystem/types/cfg/cfg.h"

class PKB {
 private:
  std::shared_ptr<FollowStore> m_follow_store;
  std::shared_ptr<ParentStore> m_parent_store;
  std::shared_ptr<UsageStore> m_usage_store;
  std::shared_ptr<ModifyStore> m_modify_store;
  std::shared_ptr<CallStore> m_call_store;
  std::shared_ptr<PatternStore> m_pattern_store;
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;
  std::shared_ptr<std::vector<std::unordered_map<std::string, std::unordered_set<std::string>>>> m_name_to_stmt;
  std::shared_ptr<std::vector<std::unordered_map<std::string, std::string>>> m_stmt_to_name;
  std::shared_ptr<Cfg> m_program_cfg;

 public:
  PKB();
  void InitRelationshipStores();
  void AddStmt(std::string const &name, StmtType type);
  void AddNameToStmt(StmtType type, std::string const &name, std::string const &stmt);
  void AddStmtToName(StmtType type, std::string const &stmt, std::string const &name);
  void AddProgramCfg(std::shared_ptr<Cfg> program_cfg);
  [[nodiscard]] std::unordered_set<std::string> GetStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::string> GetStmtByName(StmtType type, std::string name);
  [[nodiscard]] std::string GetNameByStmt(StmtType type, std::string stmt);
  [[nodiscard]] std::shared_ptr<CallStore> GetCallStore();
  [[nodiscard]] std::shared_ptr<FollowStore> GetFollowStore();
  [[nodiscard]] std::shared_ptr<ModifyStore> GetModifyStore();
  [[nodiscard]] std::shared_ptr<ParentStore> GetParentStore();
  [[nodiscard]] std::shared_ptr<UsageStore> GetUsageStore();
  [[nodiscard]] std::shared_ptr<PatternStore> GetPatternStore();
};

#endif //PKB_H
