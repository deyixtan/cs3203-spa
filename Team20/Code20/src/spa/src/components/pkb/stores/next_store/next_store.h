#ifndef NEXT_STORE_H
#define NEXT_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class ParentStore;
class FollowsStore;

class NextStore : public StmtStmtStore {
 private:
  std::shared_ptr<ParentStore> m_parent_store;
  std::unordered_map<std::string, std::string> m_proc_stmt_map;

 public:
  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                     std::shared_ptr<ParentStore> parent_store);
  void AddNext(IDENT const &before, IDENT const &next);
  void AddStmtProc(std::string const &proc, std::string const &stmt);
  void WipeNextStar();
  [[nodiscard]] bool IsNextPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsNextStarValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT_SET GetBeforeOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetNextOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetBeforeStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetNextStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetNextPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetNextStarPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllNextStmt(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_SET GetNextStarSameStmt(StmtType type);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllNextStarStmt(StmtType type1, StmtType type2);
};

#endif //NEXT_STORE_H
