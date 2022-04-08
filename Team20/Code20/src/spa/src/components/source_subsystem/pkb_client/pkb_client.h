#ifndef PKB_CLIENT_H
#define PKB_CLIENT_H

#include "components/pkb/common.h"
#include "components/source_subsystem/source_declarations.h"

namespace source {

class PkbClient {
 private:
  PkbPtr m_pkb;

 public:
  explicit PkbClient(PkbPtr pkb);
  StringSet GetVarUsedByStmt(String stmt);
  StringSet GetVarModByStmt(String stmt);
  StringSet GetCallStmtOf(String stmt);
  StringSet GetAllAnceOf(String stmt);
  StringSet GetCallersOf(String stmt);

  CfgPtr GetProgramCfg();
  void UpdateCallUsesModifies(String &proc);
  void UpdateCallUses(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers);
  void UpdateCallModifies(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers);

  void PopulateParent(String stmt1, String stmt2);
  void PopulateParentStar(String stmt, StringStream visited);
  void PopulateFollows(String stmt1, String stmt2);
  void PopulateFollowsStar(String stmt1, String stmt2);
  void PopulateUses(String stmt, String var);
  void PopulateModifies(String stmt, String var);
  void PopulateCalls(String caller, String callee);
  void PopulateProc(String name);

  void PopulateAssign(StringStream &visited,
                      String &proc_name,
                      String &curr_stmt,
                      String &var_name,
                      String &rhs_expr);
  void AssignHelper(StringStream &visited,
                    String &proc_name,
                    String &curr_stmt,
                    String &var_name,
                    String &rhs_expr);

  void PopulateStmt(String stmt);
  void PopulateTypeOfStmt(String stmt, StmtType type);
  StmtType GetTypeOfStmt(String stmt);
  void PopulateName(String name, StmtType type);

  void PopulateRead(StringStream &visited, String &curr_stmt, String &var_name);
  void ReadHelper(StringStream &visited, String &curr_stmt, String &var_name);

  void PopulatePrint(StringStream &visited, String &curr_stmt, String &var_name);
  void PrintHelper(StringStream &visited, String &curr_stmt);

  void PopulateVars(StringStream &visited,
                    String &curr_stmt,
                    String &proc_name,
                    String &var_name,
                    bool is_uses);
  void VarsHelper(StringStream &visited,
                  String &curr_stmt,
                  String &proc_name,
                  String &var_name,
                  bool is_uses);

  void PopulateWhile(StringStream &visited, String &curr_stmt, String cond_expr);
  void WhileHelper(StringStream &visited, String &curr_stmt, String cond_expr);

  void PopulateIf(StringStream &visited, String curr_stmt, String cond_expr);
  void IfHelper(StringStream &visited, String curr_stmt, String cond_expr);

  void PopulateConst(String name);
  void PopulateCallStmt(String proc, String stmt);
  void PopulateCall(StringStream &visited,
                    String &curr_stmt,
                    String &proc_name,
                    String &callee_name);
  void PopulateCfg(CfgPtr cfg);
  void PopulateNext(String stmt1, String stmt2);
  void PopulateNextStar(String stmt1, String stmt2);
  void AddPattern(StmtType type, String stmt, String lhs, String rhs);
};

}

#endif //PKB_CLIENT_H
