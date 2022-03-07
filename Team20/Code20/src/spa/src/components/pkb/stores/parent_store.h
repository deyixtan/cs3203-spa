#ifndef PARENT_STORE_H
#define PARENT_STORE_H

#include "store.h"

// A struct to maintain every node's relationships
struct parent_child {
  std::string parent;
  std::unordered_set<std::string> child;
  std::unordered_set<std::string> ance;
  std::unordered_set<std::string> desc;
};

// A store class that maintains all Parent APIs and relationships
class ParentStore : public Store {
 public:
  explicit ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStmt(StmtType type1,
                                                                                                    StmtType type2);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllParentStarStmt(StmtType type1,
                                                                                                        StmtType type2);
  bool IsParent(std::string const &stmt);

  bool IsChild(std::string const &stmt);

  bool IsAnce(std::string const &stmt);

  bool IsDesc(std::string const &stmt);

  bool ParentChildExists(std::string const &stmt1, std::string const &stmt2);

  bool AnceExists(std::string const &curr, std::string const &ance);

  bool DescExists(std::string const &curr, std::string const &desc);

  void Init(int num_stmts);

  void AddParent(std::string const &parent, std::string const &child);

  void AddParentStar(std::string const &stmt, std::vector<std::string> const &visited);

  std::unordered_set<std::string> GetAllParents();

  std::string GetParentOf(std::string const &stmt);

  std::unordered_set<std::string> GetChildOf(std::string const &stmt);

  std::unordered_set<std::string> GetAllAnceOf(std::string const &stmt);

  std::unordered_set<std::string> GetAllDescOf(std::string const &stmt);

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetParentChildPairs();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAnceDescPairs();

 private:
  std::unordered_map<std::string, parent_child> rs_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> parent_child_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> ance_desc_set;
  std::unordered_set<std::string> ance_set;
  std::unordered_set<std::string> desc_set;
  std::unordered_set<std::string> parent_set;
  std::unordered_set<std::string> child_set;
};
#endif //PARENT_STORE_H
