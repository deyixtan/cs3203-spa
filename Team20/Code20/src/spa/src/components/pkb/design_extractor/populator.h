#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_DESIGN_EXTRACTOR_POPULATOR_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_DESIGN_EXTRACTOR_POPULATOR_H_

#include "../pkb.h"
#include "components/source_subsystem/ast/node_statement.h"

class Populator {
 private:
  PKB *pkb;

 public:
  Populator(PKB *pkb);
  void PopulateParent(std::string stmt1, std::string stmt2);
  void PopulateParentStar(std::string stmt, std::vector<std::string> visited);
  void PopulateFollows(std::string stmt1, std::string stmt2);
  void PopulateFollowsStar(std::vector<std::shared_ptr<StatementNode>> stmts_lst, int index);
  void PopulateUses(std::string stmt, std::string var);
  void PopulateModifies(std::string stmt, std::string var);
  void PopulateProc(std::string name);
  void PopulateAssign(std::string stmt);
  void PopulateStmt(std::string stmt);
  void PopulateRead(std::string stmt);
  void PopulatePrint(std::string stmt);
  void PopulateVars(std::string var);
  void PopulateWhile(std::string stmt);
  void PopulateIf(std::string stmt);
  void PopulateConst(std::string name);
  void AddPattern(std::string stmt, std::string lhs, std::string rhs);
};
#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_DESIGN_EXTRACTOR_POPULATOR_H_
