#ifndef SPA_SRC_COMPONENTS_PKB_DESIGN_EXTRACTOR_POPULATOR_H_
#define SPA_SRC_COMPONENTS_PKB_DESIGN_EXTRACTOR_POPULATOR_H_

#include "../pkb/pkb.h"
#include "cfg/cfg_program_node.h"

class Populator {
 private:
  PKB *pkb;

 public:
  explicit Populator(PKB *pkb);
  void PopulateParent(std::string stmt1, std::string stmt2);
  void PopulateParentStar(std::string stmt, std::vector<std::string> visited);
  void PopulateFollows(std::string stmt1, std::string stmt2);
  void PopulateFollowsStar(std::string stmt1, std::string stmt2);
  void PopulateUses(std::string stmt, std::string var);
  void PopulateUsesProc(std::string stmt, std::string var);
  void PopulateModifies(std::string stmt, std::string var);
  void PopulateModifiesProc(std::string stmt, std::string var);
  void PopulateProc(std::string name);
  void PopulateAssign(std::string stmt);
  void PopulateStmt(std::string stmt);
  void PopulateRead(std::string stmt);
  void PopulatePrint(std::string stmt);
  void PopulateVars(std::string var);
  void PopulateWhile(std::string stmt);
  void PopulateIf(std::string stmt);
  void PopulateConst(std::string name);
  void AddStmtPattern(std::string stmt, std::string lhs, std::string rhs);
  void AddWhilePattern(std::string stmt, std::string expr);
  void AddIfPattern(std::string stmt, std::string expr);
  void PopulateCfg(source::CfgProgramNode &cfg);
};

#endif //SPA_SRC_COMPONENTS_PKB_DESIGN_EXTRACTOR_POPULATOR_H_
