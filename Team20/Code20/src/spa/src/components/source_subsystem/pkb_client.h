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
  void PopulateAssign(std::string stmt);
  void PopulateStmt(std::string stmt);
  void PopulateRead(std::string stmt, std::string name);
  void PopulatePrint(std::string stmt, std::string name);
  void PopulateVars(std::string var);
  void PopulateWhile(std::string stmt);
  void PopulateIf(std::string stmt);
  void PopulateConst(std::string name);
  void PopulateCall(std::string stmt, std::string name);
  void PopulateCfg(Cfg &cfg);
  void AddPattern(StmtType type, std::string stmt, std::string lhs, std::string rhs);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_PKB_CLIENT_H_
