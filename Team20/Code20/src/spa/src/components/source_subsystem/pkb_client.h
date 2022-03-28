#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_PKB_CLIENT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_PKB_CLIENT_H_

#include <string>

#include "../pkb/stores/store.h"

class PKB;
class Cfg;

class PkbClient {
 private:
  PKB *pkb;

 public:
  explicit PkbClient(PKB *pkb);
  PKB *GetPKB();
  void PopulateParent(std::string stmt1, std::string stmt2);
  void PopulateParentStar(std::string stmt, std::vector<std::string> visited);
  void PopulateFollows(std::string stmt1, std::string stmt2);
  void PopulateFollowsStar(std::string stmt1, std::string stmt2);
  void PopulateUses(std::string stmt, std::string var);
  void PopulateUsesProc(std::string stmt, std::string var);
  void PopulateModifies(std::string stmt, std::string var);
  void PopulateModifiesProc(std::string stmt, std::string var);
  void PopulateCalls(std::string caller, std::string callee);
  void PopulateProc(std::string name);
  void PopulateAssign(std::vector<std::string> &visited,
                      std::string &proc_name,
                      std::string &curr_stmt,
                      std::string &var_name,
                      std::string &rhs_expr);
  void PopulateStmt(std::string stmt);
  void PopulateName(std::string name, StmtType type);
  void PopulateRead(std::string stmt, std::string name);
  void PopulatePrint(std::string stmt, std::string name);
  void PopulateVars(std::vector<std::string> &visited,
                    std::string &curr_stmt,
                    std::string &proc_name,
                    std::string &var_name,
                    bool is_uses);
  void PopulateWhile(std::vector<std::string> &visited, std::string &curr_stmt, std::string cond_expr);
  void PopulateIf(std::string stmt);
  void PopulateConst(std::string name);
  void PopulateCallStmt(std::string proc, std::string stmt);
  void PopulateCall(std::string stmt, std::string name);
  void PopulateCfg(Cfg &cfg);
  void PopulateNext(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map);
  void AddPattern(StmtType type, std::string stmt, std::string lhs, std::string rhs);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_PKB_CLIENT_H_
