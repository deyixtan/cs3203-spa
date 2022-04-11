#ifndef PKB_CLIENT_H
#define PKB_CLIENT_H

#include "components/pkb/pkb_declarations.h"
#include "components/source_subsystem/source_declarations.h"

namespace pkb {

class PkbClient {
 private:
  pkb::PkbPtr m_pkb;

 private:
  void ReadHelper(StringStream &visited, String &curr_stmt, String &var_name);
  void PrintHelper(StringStream &visited, String &curr_stmt);
  void AssignHelper(StringStream &visited, String &proc_name, String &curr_stmt, String &var_name, String &rhs_expr);
  void PopulateCallStmt(String proc, String stmt);
  void UpdateCallUses(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers);
  void UpdateCallModifies(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers);
  void WhileHelper(StringStream &visited, String &curr_stmt, String cond_expr);
  void IfHelper(StringStream &visited, String curr_stmt, String cond_expr);
  void VarsHelper(StringStream &visited, String &curr_stmt, String &proc_name, String &var_name, bool is_uses);

 public:
  explicit PkbClient(pkb::PkbPtr pkb);

  // getters
  [[nodiscard]] source::CfgPtr GetProgramCfg();
  [[nodiscard]] StmtType GetTypeOfStmt(String stmt);
  [[nodiscard]] StringSet GetVarUsedByStmt(String stmt);
  [[nodiscard]] StringSet GetVarModByStmt(String stmt);
  [[nodiscard]] StringSet GetCallersOf(String stmt);
  [[nodiscard]] StringSet GetCallStmtOf(String stmt);
  [[nodiscard]] StringSet GetAllAnceOf(String stmt);

  // setters
  void PopulateCfg(source::CfgPtr cfg);
  void PopulateProc(String name);
  void PopulateName(String name, StmtType type);
  void PopulateTypeOfStmt(String stmt, StmtType type);
  void PopulateStmt(String stmt);
  void PopulateRead(StringStream &visited, String &curr_stmt, String &var_name);
  void PopulatePrint(StringStream &visited, String &curr_stmt, String &var_name);
  void PopulateProcFirstStmt(String proc_name, String stmt);
  void PopulateProcLastStmt(String proc_name, String stmt);
  void PopulateAssign(StringStream &visited, String &proc_name, String &curr_stmt, String &var_name, String &rhs_expr);
  void PopulateCall(StringStream &visited, String &curr_stmt, String &proc_name, String &callee_name);
  void PopulateWhile(StringStream &visited, String &curr_stmt, String cond_expr);
  void PopulateIf(StringStream &visited, String curr_stmt, String cond_expr);
  void PopulateVars(StringStream &visited, String &curr_stmt, String &proc_name, String &var_name, bool is_uses);
  void PopulateConst(String name);

  // setters (clauses-related)
  void PopulateUses(String stmt, String var);
  void PopulateModifies(String stmt, String var);
  void PopulateFollows(String stmt1, String stmt2);
  void PopulateFollowsStar(String stmt1, String stmt2);
  void PopulateParent(String stmt1, String stmt2);
  void PopulateParentStar(String stmt, StringStream visited);
  void PopulateCalls(String caller, String callee);
  void UpdateCallUsesModifies(String &proc);
  void PopulateNext(String stmt1, String stmt2);

  // setters (others)
  void AddPattern(StmtType type, String stmt, String lhs, String rhs);
};

}

#endif //PKB_CLIENT_H
