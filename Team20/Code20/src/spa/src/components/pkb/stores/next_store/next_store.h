#ifndef NEXT_STORE_H
#define NEXT_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class NextStore : public StmtStmtStore {
 private:

 public:
  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddNext(IDENT const &before, IDENT const &next);
  void WipeStar();
  [[nodiscard]] bool IsNextPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsNextStarValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT_SET GetBeforeOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetNextOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetBeforeStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetNextStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_SET GetNextPairs();
  [[nodiscard]] IDENT_PAIR_SET GetNextStarPairs();
  [[nodiscard]] IDENT_PAIR_SET GetAllNextStmt(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_SET GetNextStarSameStmt(StmtType type);
  [[nodiscard]] IDENT_PAIR_SET GetAllNextStarStmt(StmtType type1, StmtType type2);
};

#endif //NEXT_STORE_H
