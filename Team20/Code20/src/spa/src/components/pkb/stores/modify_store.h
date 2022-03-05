#ifndef MODIFY_STORE_H
#define MODIFY_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "utils/pair_hash.h"


// A store class that maintains all Modifies APIs and relationships
class ModifyStore {
 public:
  ModifyStore();

  void AddStmtVar(std::string stmt, std::string var);

  void AddProcVar(std::string proc, std::string var);

  bool StmtVarExists(std::pair<std::string, std::string> pair);

  bool ProcVarExists(std::pair<std::string, std::string> pair);

  std::unordered_set<std::string> GetVarModByStmt(std::string stmt);

  std::unordered_set<std::string> GetStmtModByVar(std::string var);

  std::unordered_set<std::string> GetVarModByProc(std::string proc);

  std::unordered_set<std::string> GetProcModByVar(std::string var);

  std::unordered_set<std::string> GetAllStmtModify();

  std::unordered_set<std::string> GetAllProcModify();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllStmtVar();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllProcVar();

 private:
  std::unordered_set<std::string> all_stmt_modify;
  std::unordered_set<std::string> all_proc_modify;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> stmt_var_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> proc_var_pairs;
  std::unordered_map<std::string, std::unordered_set<std::string> > stmt_var_map;
  std::unordered_map<std::string, std::unordered_set<std::string> > var_stmt_map;
  std::unordered_map<std::string, std::unordered_set<std::string> > proc_var_map;
  std::unordered_map<std::string, std::unordered_set<std::string> > var_proc_map;
};
#endif //MODIFY_STORE_H
