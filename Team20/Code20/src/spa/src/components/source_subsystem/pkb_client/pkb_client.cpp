#include "pkb_client.h"
#include "components/pkb/pkb.h"

namespace source {

PkbClient::PkbClient(PkbPtr pkb) : m_pkb(pkb) {}

void PkbClient::ReadHelper(StringStream &visited, String &curr_stmt, String &var_name) {
  PopulateStmt(curr_stmt);
  PopulateParentStar(curr_stmt, visited);
}

void PkbClient::PrintHelper(StringStream &visited, String &curr_stmt) {
  PopulateStmt(curr_stmt);
  PopulateParentStar(curr_stmt, visited);
}

void PkbClient::AssignHelper(StringStream &visited,
                             String &proc_name,
                             String &curr_stmt,
                             String &var_name,
                             String &rhs_expr) {
  PopulateStmt(curr_stmt);
  PopulateParentStar(curr_stmt, visited);
  m_pkb->GetPatternStore()->AddStmtWithPattern(curr_stmt, var_name, rhs_expr);
}

void PkbClient::PopulateCallStmt(String proc, String stmt) {
  m_pkb->GetCallStore()->AddCallStmtMap(proc, stmt);
}

void PkbClient::UpdateCallUses(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers) {
  for (auto &var : vars) {
    PopulateUses(call_stmt, var);
    for (auto &ance : ancestors) {
      PopulateUses(ance, var);
    }
    for (auto &caller : callers) {
      PopulateUses(caller, var);
    }
  }
}

void PkbClient::UpdateCallModifies(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers) {
  for (auto &var : vars) {
    PopulateModifies(call_stmt, var);
    for (auto &ance : ancestors) {
      PopulateModifies(ance, var);
    }
    for (auto &caller : callers) {
      PopulateModifies(caller, var);
    }
  }
}

void PkbClient::WhileHelper(StringStream &visited, String &curr_stmt, String cond_expr) {
  PopulateStmt(curr_stmt);
  PopulateParentStar(curr_stmt, visited);
  m_pkb->GetPatternStore()->AddWhileWithPattern(curr_stmt, cond_expr);
}

void PkbClient::IfHelper(StringStream &visited, String curr_stmt, String cond_expr) {
  PopulateStmt(curr_stmt);
  PopulateParentStar(curr_stmt, visited);
  m_pkb->GetPatternStore()->AddIfWithPattern(curr_stmt, cond_expr);
}

void PkbClient::VarsHelper(StringStream &visited,
                           String &curr_stmt,
                           String &proc_name,
                           String &var_name,
                           bool is_uses) {
  if (!is_uses) {
    for (String s : visited) {
      PopulateModifies(s, var_name);
    }
    PopulateModifies(proc_name, var_name);
    PopulateModifies(curr_stmt, var_name);
  } else {
    for (String s : visited) {
      PopulateUses(s, var_name);
    }
    PopulateUses(proc_name, var_name);
    PopulateUses(curr_stmt, var_name);
  }
}

CfgPtr PkbClient::GetProgramCfg() {
  return m_pkb->GetProgCfg();
}

StmtType PkbClient::GetTypeOfStmt(String stmt) {
  return m_pkb->GetTypeOfStmt(stmt);
}

StringSet PkbClient::GetVarUsedByStmt(String stmt) {
  return m_pkb->GetUsesStore()->GetVarUsedByStmt(stmt);
}

StringSet PkbClient::GetVarModByStmt(String stmt) {
  return m_pkb->GetModifiesStore()->GetVarModByStmt(stmt);
}

StringSet PkbClient::GetCallersOf(String stmt) {
  return m_pkb->GetCallStore()->GetCallersOf(stmt);
}

StringSet PkbClient::GetCallStmtOf(String stmt) {
  return m_pkb->GetCallStore()->GetCallStmtOf(stmt);
}

StringSet PkbClient::GetAllAnceOf(String stmt) {
  return m_pkb->GetParentStore()->GetAllAnceOf(STMT, stmt);
}

void PkbClient::PopulateCfg(CfgPtr cfg) {
  m_pkb->AddProgramCfg(cfg);
}

void PkbClient::PopulateProc(String name) {
  m_pkb->AddStmt(name, PROC);
}

void PkbClient::PopulateProcFirstStmt(std::string proc_name, std::string stmt) {
  m_pkb->GetNextStore()->AddFirstStmtProc(proc_name, stmt);
}

void PkbClient::PopulateProcLastStmt(std::string proc_name, std::string stmt) {
  m_pkb->GetNextStore()->AddLastStmtProc(proc_name, stmt);
}

void PkbClient::PopulateName(String name, StmtType type) {
  m_pkb->AddName(name, type);
}

void PkbClient::PopulateTypeOfStmt(String stmt, StmtType type) {
  m_pkb->AddTypeOfStmt(stmt, type);
}

void PkbClient::PopulateStmt(String stmt) {
  m_pkb->AddStmt(stmt, STMT);
}

void PkbClient::PopulateRead(StringStream &visited, String &curr_stmt, String &var_name) {
  m_pkb->AddStmt(curr_stmt, READ);
  m_pkb->AddStmtToName(READ, curr_stmt, var_name);
  m_pkb->AddNameToStmt(READ, var_name, curr_stmt);
  PopulateName(var_name, READ);
  ReadHelper(visited, curr_stmt, var_name);
}

void PkbClient::PopulatePrint(StringStream &visited, String &curr_stmt, String &var_name) {
  m_pkb->AddStmt(curr_stmt, PRINT);
  m_pkb->AddStmtToName(PRINT, curr_stmt, var_name);
  m_pkb->AddNameToStmt(PRINT, var_name, curr_stmt);
  PopulateName(var_name, PRINT);
  PrintHelper(visited, curr_stmt);
}

void PkbClient::PopulateAssign(StringStream &visited,
                               String &proc_name,
                               String &curr_stmt,
                               String &var_name,
                               String &rhs_expr) {
  m_pkb->AddStmt(curr_stmt, ASSIGN);
  AssignHelper(visited, proc_name, curr_stmt, var_name, rhs_expr);
}

void PkbClient::PopulateCall(StringStream &visited,
                             String &curr_stmt,
                             String &proc_name,
                             String &callee_name) {
  m_pkb->AddStmt(curr_stmt, CALL);
  m_pkb->AddStmtToName(CALL, curr_stmt, callee_name);
  m_pkb->AddNameToStmt(CALL, callee_name, curr_stmt);
  PopulateStmt(curr_stmt);
  PopulateName(callee_name, CALL);
  PopulateCalls(proc_name, callee_name);
  PopulateCallStmt(callee_name, curr_stmt);
  PopulateParentStar(curr_stmt, visited);
}

void PkbClient::PopulateWhile(StringStream &visited, String &curr_stmt, String cond_expr) {
  m_pkb->AddStmt(curr_stmt, WHILE);
  WhileHelper(visited, curr_stmt, cond_expr);
}

void PkbClient::PopulateIf(StringStream &visited, String curr_stmt, String cond_expr) {
  m_pkb->AddStmt(curr_stmt, IF);
  IfHelper(visited, curr_stmt, cond_expr);
}

void PkbClient::PopulateVars(StringStream &visited,
                             String &curr_stmt,
                             String &proc_name,
                             String &var_name,
                             bool is_uses) {
  m_pkb->AddStmt(var_name, VARS);
  VarsHelper(visited, curr_stmt, proc_name, var_name, is_uses);
}

void PkbClient::PopulateConst(String name) {
  m_pkb->AddStmt(name, CONSTS);
}

void PkbClient::PopulateUses(String stmt, String var) {
  m_pkb->GetUsesStore()->AddStmtVar(stmt, var);
}

void PkbClient::PopulateModifies(String stmt, String var) {
  m_pkb->GetModifiesStore()->AddStmtVar(stmt, var);
}

void PkbClient::PopulateFollows(String stmt1, String stmt2) {
  m_pkb->GetFollowsStore()->AddFollow(stmt1, stmt2);
}

void PkbClient::PopulateFollowsStar(String stmt1, String stmt2) {
  m_pkb->GetFollowsStore()->AddFollowStar(stmt1, stmt2);
}

void PkbClient::PopulateParent(String stmt1, String stmt2) {
  m_pkb->GetParentStore()->AddParent(stmt1, stmt2);
}

void PkbClient::PopulateParentStar(String stmt, StringStream visited) {
  m_pkb->GetParentStore()->AddParentStar(stmt, visited);
}

void PkbClient::PopulateCalls(String caller, String callee) {
  m_pkb->GetCallStore()->AddCalls(caller, callee);
}

void PkbClient::UpdateCallUsesModifies(String &proc) {
  StringSet uses_vars = GetVarUsedByStmt(proc);
  StringSet mod_vars = GetVarModByStmt(proc);
  StringSet call_stmts = GetCallStmtOf(proc);
  StringSet callers = GetCallersOf(proc);

  for (auto call_stmt : call_stmts) {
    StringSet ancestors = GetAllAnceOf(call_stmt);
    UpdateCallUses(call_stmt, uses_vars, ancestors, callers);
    UpdateCallModifies(call_stmt, mod_vars, ancestors, callers);
  }
}

void PkbClient::PopulateNext(String stmt1, String stmt2) {
  m_pkb->GetNextStore()->AddNext(stmt1, stmt2);
}

}
