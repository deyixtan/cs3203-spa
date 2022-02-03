#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

struct hashFunction {
  size_t operator()(const std::pair<int, int> &x) const {
    return x.first ^ x.second;
  }
};

// A store class that maintains all Uses APIs and relationships
class UsageStore {
 public:
  UsageStore();

  bool add_stmt_var(int stmt, std::string var);

  bool add_proc_var(std::string proc, std::string var);

  bool stmt_var_exists(std::pair<int, std::string> pair);

  bool proc_var_exists(std::pair<std::string, std::string> pair);

  std::unordered_set<std::string> get_var_used_by_stmt(int stmt);

  std::unordered_set<int> get_stmt_used_by_var(std::string var);

  std::unordered_set<std::string> get_var_used_by_proc(std::string proc);

  std::unordered_set<std::string> get_proc_used_by_var(std::string var);

  std::unordered_set<std::pair<int, std::string>, hashFunction> get_all_stmt_var();

  std::unordered_set<std::pair<std::string, std::string>, hashFunction> get_all_proc_var();

 private:
  static std::unordered_set<std::pair<int, std::string>, hashFunction> stmt_var_pairs;
  static std::unordered_set<std::pair<std::string, std::string>, hashFunction> proc_var_pairs;
  static std::unordered_map<int, std::unordered_set<std::string> > stmt_var_map;
  static std::unordered_map<std::string, std::unordered_set<int> > var_stmt_map;
  static std::unordered_map<std::string, std::unordered_set<std::string> > proc_var_map;
  static std::unordered_map<std::string, std::unordered_set<std::string> > var_proc_map;
};
