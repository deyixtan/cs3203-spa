#include "pkb_client.h"
#include "../pkb/pkb.h"

PkbClient::PkbClient(PKB *pkb) : pkb(pkb) {}

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

void PkbClient::PopulateAssign(std::string stmt) {
  pkb->AddStmt(stmt, ASSIGN);
}

void PkbClient::PopulateStmt(std::string stmt) {
  pkb->AddStmt(stmt, STMT);
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

void PkbClient::PopulateVars(std::string var) {
  pkb->AddStmt(var, VARS);
}

void PkbClient::PopulateWhile(std::string stmt) {
  pkb->AddStmt(stmt, WHILE);
}

void PkbClient::PopulateIf(std::string stmt) {
  pkb->AddStmt(stmt, IF);
}

void PkbClient::PopulateConst(std::string name) {
  pkb->AddStmt(name, CONSTS);
}

void PkbClient::PopulateCall(std::string stmt, std::string name) {
  pkb->AddStmt(stmt, CALL);
  pkb->AddStmtToName(CALL, stmt, name);
  pkb->AddNameToStmt(CALL, name, stmt);
}

void PkbClient::PopulateCfg(Cfg &cfg) {
  pkb->AddProgramCfg(std::make_shared<Cfg>(cfg));
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
