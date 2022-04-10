#ifndef PKB_H
#define PKB_H

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "components/pkb/stores/uses_store/uses_store.h"
#include "components/pkb/stores/modifies_store/modifies_store.h"
#include "components/pkb/stores/follows_store/follows_store.h"
#include "components/pkb/stores/parent_store/parent_store.h"
#include "components/pkb/stores/call_store/call_store.h"
#include "components/pkb/stores/pattern_store/pattern_store.h"
#include "components/pkb/stores/next_store/next_store.h"
#include "components/pkb/stores/affects_store/affect_store_factory.h"
#include "components/pkb/stores/pattern_store/expression_tree/expression_tree.h"
#include "../source_subsystem/types/cfg/cfg.h"
#include "components/pkb/stores/next_store/next_store.h"

namespace pkb {

class PKB {
 private:
  std::shared_ptr<FollowsStore> m_follow_store;
  std::shared_ptr<ParentStore> m_parent_store;
  std::shared_ptr<UsesStore> m_usage_store;
  std::shared_ptr<ModifiesStore> m_modify_store;
  std::shared_ptr<CallStore> m_call_store;
  std::shared_ptr<PatternStore> m_pattern_store;
  std::shared_ptr<NextStore> m_next_store;
  AffectsStoreFactoryPtr m_affects_store_factory;

  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_stmt_vector;
  std::shared_ptr<std::unordered_map<std::string, StmtType>> m_stmt_type;
  std::shared_ptr<std::vector<std::unordered_set<std::string>>> m_name_vector;
  std::shared_ptr<std::vector<std::unordered_map<std::string, std::unordered_set<std::string>>>> m_name_to_stmt;
  std::shared_ptr<std::vector<std::unordered_map<std::string, std::string>>> m_stmt_to_name;
  std::shared_ptr<source::Cfg> m_program_cfg;

 public:
  PKB();
  void InitRelationshipStores();
  void AddStmt(std::string const &stmt, StmtType type);
  void AddTypeOfStmt(std::string const &stmt, StmtType type);
  void AddName(std::string const &name, StmtType type);
  void AddNameToStmt(StmtType type, std::string const &name, std::string const &stmt);
  void AddStmtToName(StmtType type, std::string const &stmt, std::string const &name);
  void AddProgramCfg(std::shared_ptr<source::Cfg> program_cfg);
  [[nodiscard]] std::unordered_set<std::string> GetStmt(StmtType type);
  [[nodiscard]] StmtType GetTypeOfStmt(std::string stmt_no);
  [[nodiscard]] std::unordered_set<std::string> GetName(StmtType type);
  [[nodiscard]] std::unordered_set<std::string> GetStmtByName(StmtType type, std::string name);
  [[nodiscard]] std::string GetNameByStmt(StmtType type, std::string stmt);
  [[nodiscard]] std::shared_ptr<CallStore> GetCallStore();
  [[nodiscard]] std::shared_ptr<FollowsStore> GetFollowsStore();
  [[nodiscard]] std::shared_ptr<ModifiesStore> GetModifiesStore();
  [[nodiscard]] std::shared_ptr<ParentStore> GetParentStore();
  [[nodiscard]] std::shared_ptr<UsesStore> GetUsesStore();
  [[nodiscard]] std::shared_ptr<PatternStore> GetPatternStore();
  [[nodiscard]] std::shared_ptr<NextStore> GetNextStore();
  [[nodiscard]] std::shared_ptr<AffectsStoreFactory> GetAffectsStoreFactory();
  [[nodiscard]] std::shared_ptr<source::Cfg> GetProgCfg();
};

}

#endif //PKB_H
