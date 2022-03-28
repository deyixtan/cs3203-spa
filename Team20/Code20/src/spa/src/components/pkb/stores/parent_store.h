#ifndef PARENT_STORE_H
#define PARENT_STORE_H

#include "store.h"

// A struct to maintain every node's relationships
struct ParentChildNode {
  std::string parent;
  std::unordered_set<std::string> child;
  std::unordered_set<std::string> ance;
  std::unordered_set<std::string> desc;
};

// A store class that maintains all Parent APIs and relationships
class ParentStore : public Store {
 private:
  std::unordered_map<std::string, ParentChildNode> rs_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> parent_child_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> ance_desc_set;
  std::unordered_set<std::string> ance_set;
  std::unordered_set<std::string> desc_set;
  std::unordered_set<std::string> parent_set;
  std::unordered_set<std::string> child_set;

 public:
  explicit ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddParent(std::string const &parent, std::string const &child);
  void AddParentStar(std::string const &stmt, std::vector<std::string> const &visited);
  [[nodiscard]] bool IsParent(std::string const &stmt);
  [[nodiscard]] bool IsChild(std::string const &stmt);
  [[nodiscard]] bool IsAnce(std::string const &stmt);
  [[nodiscard]] bool IsDesc(std::string const &stmt);
  [[nodiscard]] bool ParentChildExists(std::string const &stmt1, std::string const &stmt2);
  [[nodiscard]] bool IsAnceExists(std::string const &curr, std::string const &ance);
  [[nodiscard]] bool IsDescExists(std::string const &curr, std::string const &desc);
  [[nodiscard]] std::string GetParentOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetChildOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAllAnceOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::string> GetAllDescOf(std::string const &stmt);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetParentChildPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAnceDescPairs();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type1,
                                                                                                        StmtType type2);
};

#endif //PARENT_STORE_H
