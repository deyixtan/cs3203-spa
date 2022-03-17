#ifndef NEXT_STORE_H_
#define NEXT_STORE_H_

#include "store.h"

class NextStore : public Store {
 public:

  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddNext(std::string const &first, std::string const &second);
  void AddNextStar(std::string const &first, std::string const &second);
  [[nodiscard]] bool IsNext(std::string const &stmt);
  [[nodiscard]] bool IsBefore(std::string const &stmt);
  [[nodiscard]] bool IsNextStar(std::string const &stmt);
  [[nodiscard]] bool IsBeforeStar(std::string const &stmt);
  [[nodiscard]] bool NextExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool NextStarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetBeforeOf(std::string const &stmt);
  [[nodiscard]] std::string GetNextOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetBeforeStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type1,
                                                                                                        StmtType type2);
};

#endif //NEXT_STORE_H_
