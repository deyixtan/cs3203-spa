#ifndef AFFECTS_SESSION_H
#define AFFECTS_SESSION_H

#include "components/pkb/common.h"
#include "components/pkb/stores/stmt_stmt_store.h"
#include "components/source_subsystem/source_declarations.h"

class AffectsStore;

class AffectsSession : StmtStmtStore {
 private:
  AffectsStorePtr m_affects_store;
  IDENT_PAIR_VECTOR m_same_affects_pairs; // for same synonym
  IDENT_PAIR_VECTOR m_same_affects_star_pairs; // for same synonym
  IDENT_PAIR_SET m_all_affects_pairs; // keeps track of duplication
  IDENT_PAIR_SET m_all_affects_star_pairs; // keeps track of duplication
  IDENT_SET_MAP_PTR_STACK m_last_modified_map_stack;
  IDENT_SET_MAP m_last_modified_star_map;
  std::stack<source::CfgNodePtr> m_terminating_node_stack;
  bool m_is_affects_star_involved;

 private:
  [[nodiscard]] IDENT_SET GetVarModByStmt(IDENT &stmt_no);
  [[nodiscard]] IDENT_SET GetVarUsedByStmt(IDENT &stmt_no);
  [[nodiscard]] IDENT GetFollowingOf(IDENT &stmt_no);
  void TraverseCfg();
  void TraverseCfg(source::CfgNodePtr &cfg_node);
  void HandleAssignStatement(IDENT stmt_no);
  void HandleReadStatement(IDENT stmt_no);
  void HandleCallStatement(IDENT stmt_no);
  void HandleWhileStatement(source::CfgNodePtr &cfg_node);
  void HandleIfStatement(IDENT stmt_no, source::CfgNodePtr &cfg_node);

 public:
  explicit AffectsSession(IDENT_SET_VECTOR_PTR stmt_vector,
                          IDENT_TO_STMT_TYPE_MAP_PTR stmt_type,
                          AffectsStorePtr affects_store,
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
  [[nodiscard]] IDENT_PAIR_VECTOR GetAffectsSameSynPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAffectsStarSameSynPairs();
};

#endif //AFFECTS_SESSION_H
