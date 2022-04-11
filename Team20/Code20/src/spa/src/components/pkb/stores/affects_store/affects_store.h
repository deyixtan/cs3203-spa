#ifndef AFFECTS_STORE_H
#define AFFECTS_STORE_H

#include "components/pkb/pkb_declarations.h"
#include "components/pkb/pkb_relationship.h"
#include "components/pkb/stores/stmt_stmt_store.h"
#include "components/source_subsystem/source_declarations.h"

namespace pkb {

class AffectsStore : StmtStmtStore {
 private:
  AffectsStoreFactoryPtr m_affects_store_factory;
  bool m_is_affects_star_involved;
  // keeps track of store states
  IDENT_SET m_same_affects_set; // for same synonym
  IDENT_SET m_same_affects_star_set; // for same synonym
  IDENT_PAIR_SET m_all_affects_pairs; // keeps track of duplication
  IDENT_PAIR_SET m_all_affects_star_pairs; // keeps track of duplication
  IDENT_SET_MAP_PTR_STACK m_last_modified_map_stack;
  IDENT_SET_MAP m_last_modified_star_map;
  std::stack<source::CfgNodePtr> m_terminating_node_stack;

 private:
  [[nodiscard]] IDENT_SET GetVarModByStmt(IDENT &stmt_no);
  [[nodiscard]] IDENT_SET GetVarUsedByStmt(IDENT &stmt_no);
  [[nodiscard]] unsigned int GetMaxPairSize();
  static void InsertToIdentSetMap(IDENT_SET_MAP &map, IDENT &key, IDENT &value);
  static void MergeModifiedTable(IDENT_SET_MAP_PTR &table1, IDENT_SET_MAP_PTR &table2);
  void UpdateModifiedTable(IDENT &stmt_no, bool is_clear_only);
  void HandleAffectsLastModSet(IDENT &stmt_no, IDENT &var_used, IDENT_SET_MAP_PTR &last_modified_map);
  void HandleAffectsStarLastModSet(IDENT &last_mod_stmt_no, IDENT &stmt_no);
  void HandleAffectsStarLastModStarSet(IDENT &last_mod_stmt_no, IDENT &stmt_no);
  void HandleWhileStatementLoop(source::CfgNodePtr &cfg_node,
                                  unsigned int &prev_size,
                                  unsigned int &curr_size,
                                  bool is_get_current);
  void AddAffects(bool is_star, IDENT &upper, IDENT &lower);
  void HandleCfg();
  void HandleCfg(source::CfgNodePtr &cfg_node);
  void HandleAssignStatement(IDENT &stmt_no);
  void HandleModifiableStatement(IDENT &stmt_no);
  void HandleWhileStatement(source::CfgNodePtr &cfg_node);
  void HandleIfStatement(source::CfgNodePtr &cfg_node);

 public:
  explicit AffectsStore(IDENT_SET_VECTOR_PTR stmt_vector,
                        IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                        AffectsStoreFactoryPtr affects_store_factory,
                        bool is_affects_star_involved);
  [[nodiscard]] bool IsAffected(IDENT const &stmt);
  [[nodiscard]] bool IsAffectedStar(IDENT const &stmt);
  [[nodiscard]] bool IsAffecting(IDENT const &stmt);
  [[nodiscard]] bool IsAffectingStar(IDENT const &stmt);
  [[nodiscard]] bool DoesAffectExists(IDENT_PAIR const &pair);
  [[nodiscard]] bool DoesAffectStarExists(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT_SET GetAffectedOf(IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetAffectedStarOf(IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetAffectsOf(IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetAffectsStarOf(IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAffectsPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAffectsStarPairs();
  [[nodiscard]] IDENT_SET GetAffectsSameSynSet();
  [[nodiscard]] IDENT_SET GetAffectsStarSameSynSet();
};

}

#endif //AFFECTS_STORE_H
