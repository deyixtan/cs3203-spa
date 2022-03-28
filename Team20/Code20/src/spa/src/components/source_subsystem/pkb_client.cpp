#include "pkb_client.h"
#include "../pkb/pkb.h"

PkbClient::PkbClient(PKB *pkb) : pkb(pkb) {}

PKB *PkbClient::GetPKB() {
  return pkb;
}

void PkbClient::PopulateParent(std::string stmt1, std::string stmt2) {
  pkb->GetParentStore()->AddParent(stmt1, stmt2);
}

void PkbClient::PopulateParentStar(std::string stmt, std::vector<std::string> visited) {
  pkb->GetParentStore()->AddParentStar(stmt, visited);
}

void PkbClient::PopulateFollows(std::string stmt1, std::string stmt2) {
  pkb->GetFollowStore()->AddFollow(stmt1, stmt2);
}

void PkbClient::PopulateFollowsStar(std::string stmt1, std::string stmt2) {
  pkb->GetFollowStore()->AddFollowStar(stmt1, stmt2);
}

void PkbClient::PopulateUses(std::string stmt, std::string var) {
  pkb->GetUsageStore()->AddStmtVar(stmt, var);
}

void PkbClient::PopulateUsesProc(std::string proc, std::string var) {
  pkb->GetUsageStore()->AddProcVar(proc, var);
}

void PkbClient::PopulateModifies(std::string stmt, std::string var) {
  pkb->GetModifyStore()->AddStmtVar(stmt, var);
}

void PkbClient::PopulateModifiesProc(std::string proc, std::string var) {
  pkb->GetModifyStore()->AddProcVar(proc, var);
}

void PkbClient::PopulateCalls(std::string caller, std::string callee) {
  pkb->GetCallStore()->AddCallerHelper(caller, callee);
}

void PkbClient::PopulateProc(std::string name) {
  pkb->AddStmt(name, PROC);
}

void PkbClient::PopulateAssign(std::vector<std::string> &visited,
                               std::string &proc_name,
                               std::string &curr_stmt,
                               std::string &var_name,
                               std::string &rhs_expr) {
  pkb->AddStmt(curr_stmt, ASSIGN);
  AssignHelper(visited, proc_name, curr_stmt, var_name, rhs_expr);
}

void PkbClient::AssignHelper(std::vector<std::string> &visited,
                             std::string &proc_name,
                             std::string &curr_stmt,
                             std::string &var_name,
                             std::string &rhs_expr) {
  PopulateStmt(curr_stmt);
  AddPattern(STMT, curr_stmt, var_name, rhs_expr);
  PopulateParentStar(curr_stmt, visited);
}

void PkbClient::PopulateStmt(std::string stmt) {
  pkb->AddStmt(stmt, STMT);
}

void PkbClient::PopulateName(std::string name, StmtType type) {
  pkb->AddName(name, type);
}

void PkbClient::PopulateRead(std::string stmt, std::string name) {
  pkb->AddStmt(stmt, READ);
  pkb->AddStmtToName(READ, stmt, name);
  pkb->AddNameToStmt(READ, name, stmt);
}

void PkbClient::PopulatePrint(std::string stmt, std::string name) {
  pkb->AddStmt(stmt, PRINT);
  pkb->AddStmtToName(PRINT, stmt, name);
  pkb->AddNameToStmt(PRINT, name, stmt);
}

void PkbClient::PopulateVars(std::vector<std::string> &visited,
                             std::string &curr_stmt,
                             std::string &proc_name,
                             std::string &var_name,
                             bool is_uses) {
  pkb->AddStmt(var_name, VARS);
  VarsHelper(visited, curr_stmt, proc_name, var_name, is_uses);
}

void PkbClient::VarsHelper(std::vector<std::string> &visited,
                           std::string &curr_stmt,
                           std::string &proc_name,
                           std::string &var_name,
                           bool is_uses) {
  if (!is_uses) {
    for (std::string s : visited) {
      PopulateModifies(s, var_name);
    }
    PopulateModifiesProc(proc_name, var_name);
    PopulateModifies(curr_stmt, var_name);
  } else {
    for (std::string s : visited) {
      PopulateUses(s, var_name);
    }
    PopulateUsesProc(proc_name, var_name);
    PopulateUses(curr_stmt, var_name);
  }
}

void PkbClient::PopulateWhile(std::vector<std::string> &visited, std::string &curr_stmt, std::string cond_expr) {
  pkb->AddStmt(curr_stmt, WHILE);
  WhileHelper(visited, curr_stmt, cond_expr);

}

void PkbClient::WhileHelper(std::vector<std::string> &visited, std::string &curr_stmt, std::string cond_expr) {
  PopulateStmt(curr_stmt);
  AddPattern(WHILE, curr_stmt, cond_expr, "");
  PopulateParentStar(curr_stmt, visited);
  visited.pop_back();
}

void PkbClient::PopulateIf(std::string stmt) {
  pkb->AddStmt(stmt, IF);
}

void PkbClient::PopulateConst(std::string name) {
  pkb->AddStmt(name, CONSTS);
}

void PkbClient::PopulateCall(std::vector<std::string> &visited,
                             std::string &curr_stmt,
                             std::string &proc_name,
                             std::string &callee_name) {
  pkb->AddStmt(curr_stmt, CALL);
  pkb->AddStmtToName(CALL, curr_stmt, callee_name);
  pkb->AddNameToStmt(CALL, callee_name, curr_stmt);
  PopulateStmt(curr_stmt);
  PopulateName(callee_name, CALL);
  PopulateCalls(proc_name, callee_name);
  PopulateCallStmt(callee_name, curr_stmt);
  PopulateParentStar(curr_stmt, visited);
}

void PkbClient::PopulateCallStmt(std::string proc, std::string stmt) {
  pkb->GetCallStore()->AddCallStmtMap(proc, stmt);
}

void PkbClient::PopulateCfg(Cfg &cfg) {
  pkb->AddProgramCfg(std::make_shared<Cfg>(cfg));
}

void PkbClient::PopulateNext(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map) {
  pkb->GetNextStore()->AddNextMap(rs_map);
  pkb->GetNextStore()->AddBeforeMap(rs_map);
  pkb->GetNextStore()->ConstructNextPairs();
}

void PkbClient::AddPattern(StmtType type, std::string stmt, std::string lhs, std::string rhs) {
  if (type == StmtType::STMT) {
    pkb->GetPatternStore()->AddStmtWithPattern(stmt, lhs, rhs);
  } else if (type == StmtType::WHILE) {
    pkb->GetPatternStore()->AddWhileWithPattern(stmt, lhs);
  } else if (type == StmtType::IF) {
    pkb->GetPatternStore()->AddIfWithPattern(stmt, lhs);
  }
}
