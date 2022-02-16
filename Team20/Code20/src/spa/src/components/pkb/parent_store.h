#ifndef PARENT_STORE_H
#define PARENT_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "hash.h"
#include <vector>

// A struct to maintain every node's relationships
struct parent_child {
  std::string parent;
  std::string child;
  std::unordered_set<std::string> ance;
  std::unordered_set<std::string> desc;
};

// A store class that maintains all Parent APIs and relationships
class ParentStore {
 public:
  ParentStore();

  bool IsParent(std::string stmt);

  bool IsChild(std::string stmt);

  bool IsAnce(std::string stmt);

  bool IsDesc(std::string stmt);

  bool ParentChildExists(std::string stmt1, std::string stmt2);

  bool AnceExists(std::string curr, std::string ance);

  bool DescExists(std::string curr, std::string desc);

  void Init(int num_stmts);

  void AddParentStmt(std::string parent, std::string child);

  void AddParentStarStmt(std::string stmt, std::vector<std::string> visited);

  std::string GetParentOf(std::string stmt);

  std::string GetChildOf(std::string stmt);

  std::unordered_set<std::string> GetAllAnceOf(std::string stmt);

  std::unordered_set<std::string> GetAllDescOf(std::string stmt);

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetParentChildPairs();

 private:
  std::unordered_map<std::string, parent_child> rs_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> parent_child_set;
  std::unordered_set<std::string> ance_set;
  std::unordered_set<std::string> desc_set;
  std::unordered_set<std::string> parent_set;
  std::unordered_set<std::string> child_set;
};
#endif //PARENT_STORE_H
