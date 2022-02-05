#ifndef USAGE_STORE_H
#define USAGE_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator () (const std::pair<T1,T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    // Mainly for demonstration purposes, i.e. works but is overly simple
    // In the real world, use sth. like boost.hash_combine
    return h1 ^ h2;
  }
};

// A store class that maintains all Uses APIs and relationships
class UsageStore {
 public:
  UsageStore();

  void add_stmt_var(int stmt, std::string var);

  void add_proc_var(std::string proc, std::string var);

  bool stmt_var_exists(std::pair<int, std::string> pair);

  bool proc_var_exists(std::pair<std::string, std::string> pair);

  std::unordered_set<std::string> get_var_used_by_stmt(int stmt);

  std::unordered_set<int> get_stmt_used_by_var(std::string var);

  std::unordered_set<std::string> get_var_used_by_proc(std::string proc);

  std::unordered_set<std::string> get_proc_used_by_var(std::string var);

  std::unordered_set<std::pair<int, std::string>, pair_hash> get_all_stmt_var();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> get_all_proc_var();

 private:
  static std::unordered_set<std::pair<int, std::string>, pair_hash> stmt_var_pairs;
  static std::unordered_set<std::pair<std::string, std::string>, pair_hash> proc_var_pairs;
  static std::unordered_map<int, std::unordered_set<std::string> > stmt_var_map;
  static std::unordered_map<std::string, std::unordered_set<int> > var_stmt_map;
  static std::unordered_map<std::string, std::unordered_set<std::string> > proc_var_map;
  static std::unordered_map<std::string, std::unordered_set<std::string> > var_proc_map;
};
#endif //USAGE_STORE_H
