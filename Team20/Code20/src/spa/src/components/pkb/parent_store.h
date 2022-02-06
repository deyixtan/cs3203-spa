#ifndef PARENT_STORE_H
#define PARENT_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "hash.h"

// A struct to maintain every node's relationships
struct parent_child {
  int parent;
  int child;
  std::unordered_set<int> ance;
  std::unordered_set<int> desc;
};

// A store class that maintains all Parent APIs and relationships
class ParentStore {
 public:
  ParentStore();

  bool is_parent(int stmt);

  bool is_child(int stmt);

  bool is_ance(int stmt);

  bool is_desc(int stmt);

  bool parent_child_exists(int stmt1, int stmt2);

  bool ance_exists(int curr, int ance);

  bool desc_exists(int curr, int desc);

  void rs_init(int num_stmts);

  void add_parent_stmt(int parent, int child);

  int get_parent_of(int stmt);

  int get_child_of(int stmt);

  std::unordered_set<int> get_all_ance_of(int stmt);

  std::unordered_set<int> get_all_desc_of(int stmt);

  std::unordered_set<std::pair<int, int>, pair_hash> get_parent_child_pairs();

 private:
  std::unordered_map<int, parent_child> rs_map;
  std::unordered_set<std::pair<int, int>, pair_hash> parent_child_set;
  std::unordered_set<int> ance_set;
  std::unordered_set<int> desc_set;
  std::unordered_set<int> parent_set;
  std::unordered_set<int> child_set;
};
#endif //PARENT_STORE_H
