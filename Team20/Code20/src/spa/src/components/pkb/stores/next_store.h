#ifndef NEXT_STORE_H_
#define NEXT_STORE_H_

#include "store.h"

class NextStore : public Store {
 private:
  std::unordered_map<std::string, std::unordered_set<std::string>> next_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> before_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_next_pairs;

 public:
  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddNextMap(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map);
  void AddBeforeMap(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map);
  void ConstructNextPairs();
  void FindNextStarOf(std::string const &stmt, std::unordered_set<std::string> &res, std::unordered_set<std::string> &visited);
  void FindBeforeStarOf(std::string const &stmt, std::unordered_set<std::string> &res, std::unordered_set<std::string> &visited) ;
  [[nodiscard]] bool IsNext(std::string const &stmt);
  [[nodiscard]] bool IsBefore(std::string const &stmt);
  [[nodiscard]] bool IsNextStar(std::string const &stmt);
  [[nodiscard]] bool IsBeforeStar(std::string const &stmt);
  [[nodiscard]] bool IsNextExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsNextStarExists(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetBeforeOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetBeforeStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextStarOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextStarSameStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type1,
                                                                                                        StmtType type2);
};

#endif //NEXT_STORE_H_
