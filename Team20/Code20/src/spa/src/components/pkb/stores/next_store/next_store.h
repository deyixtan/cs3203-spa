#ifndef NEXT_STORE_H
#define NEXT_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class ParentStore;
class FollowsStore;

class NextStore : public StmtStmtStore {
 private:
  std::shared_ptr<ParentStore> m_parent_store;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_proc_stmt_map;

 public:
  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                     std::shared_ptr<ParentStore> parent_store);
  void AddNext(std::string const &before, std::string const &next);
  void AddStmtProc(std::string const &proc, std::string const &stmt);
  [[nodiscard]] std::shared_ptr<ParentStore> GetParentStore();
  [[nodiscard]] bool IsNextPairValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsNextStarValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetStmtProc();
  [[nodiscard]] std::unordered_set<std::string> GetBeforeOf(StmtType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextOf(StmtType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetBeforeStarOf(StmtType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetNextStarOf(StmtType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetNextStarPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStmt(StmtType type1,
                                                                                                  StmtType type2);
  [[nodiscard]] std::unordered_set<std::string> GetNextStarSameStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllNextStarStmt(StmtType type1,
                                                                                                      StmtType type2);
};

#endif //NEXT_STORE_H
