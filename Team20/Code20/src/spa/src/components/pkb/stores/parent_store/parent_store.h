#ifndef PARENT_STORE_H
#define PARENT_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

// A store class that maintains all Parent APIs and relationships
class ParentStore : public StmtStmtStore {
 private:

 public:
  explicit ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                       std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddParent(std::string const &parent, std::string const &child);
  void AddParentStar(std::string const &stmt, std::vector<std::string> const &visited);
  [[nodiscard]] bool IsParentPairValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] std::string GetParentOf(StmtType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetChildOf(StmtType type, std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAllAnceOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAllDescOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetParentChildPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAnceDescPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type1,
                                                                                                        StmtType type2);
};

#endif //PARENT_STORE_H
