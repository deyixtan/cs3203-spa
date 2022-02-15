#ifndef USAGE_STORE_H
#define USAGE_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "hash.h"

// A store class that maintains all Uses APIs and relationships
class UsageStore {
 public:
  UsageStore();

  void AddStmtVar(std::string stmt, std::string var);

  void AddProcVar(std::string proc, std::string var);

  bool StmtVarExists(std::pair<std::string, std::string> pair);

  bool ProcVarExists(std::pair<std::string, std::string> pair);

  std::unordered_set<std::string> GetVarUsedByStmt(std::string stmt);

  std::unordered_set<std::string> GetStmtUsedByVar(std::string var);

  std::unordered_set<std::string> GetVarUsedByProc(std::string proc);

  std::unordered_set<std::string> GetProcUsedByVar(std::string var);

  std::unordered_set<std::string> GetAllStmtUsing();

  std::unordered_set<std::string> GetAllProcUsing();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmtVar();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllProcVar();

 private:
  std::unordered_set<std::string> all_stmt_using;
  std::unordered_set<std::string> all_proc_using;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> stmt_var_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> proc_var_pairs;
  std::unordered_map<std::string, std::unordered_set<std::string> > stmt_var_map;
  std::unordered_map<std::string, std::unordered_set<std::string> > var_stmt_map;
  std::unordered_map<std::string, std::unordered_set<std::string> > proc_var_map;
  std::unordered_map<std::string, std::unordered_set<std::string> > var_proc_map;
};
#endif //USAGE_STORE_H
