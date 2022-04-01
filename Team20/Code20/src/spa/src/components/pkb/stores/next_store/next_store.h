#ifndef NEXT_STORE_H
#define NEXT_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class NextStore : public StmtStmtStore {
 private:

 public:
  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddNext(std::string const &before, std::string const &next);
  void WipeStar();
  [[nodiscard]] bool IsNext(std::string const &stmt);
  [[nodiscard]] bool IsBefore(std::string const &stmt);
  [[nodiscard]] bool IsNextStar(std::string const &stmt);
  [[nodiscard]] bool IsBeforeStar(std::string const &stmt);
  [[nodiscard]] bool IsNextValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsNextStarValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetBeforeOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetBeforeStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type1,
                                                                                                  StmtType type2);
  [[nodiscard]] std::unordered_set<std::string> GetNextStarSameStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type1,
                                                                                                      StmtType type2);
};

#endif //NEXT_STORE_H
