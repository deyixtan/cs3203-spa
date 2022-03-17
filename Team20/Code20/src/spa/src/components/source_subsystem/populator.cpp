#include "populator.h"

Populator::Populator(PKB *pkb) : pkb(pkb) {}

void Populator::PopulateParent(std::string stmt1, std::string stmt2) {
  pkb->GetParentStore()->AddParent(stmt1, stmt2);
}

void Populator::PopulateParentStar(std::string stmt, std::vector<std::string> visited) {
  pkb->GetParentStore()->AddParentStar(stmt, visited);
}

void Populator::PopulateFollows(std::string stmt1, std::string stmt2) {
  pkb->GetFollowStore()->AddFollow(stmt1, stmt2);
}

void Populator::PopulateFollowsStar(std::string stmt1, std::string stmt2) {
  pkb->GetFollowStore()->AddFollowStar(stmt1, stmt2);
}

void Populator::PopulateUses(std::string stmt, std::string var) {
  pkb->GetUsageStore()->AddStmtVar(stmt, var);
}

void Populator::PopulateUsesProc(std::string proc, std::string var) {
  pkb->GetUsageStore()->AddProcVar(proc, var);
}

void Populator::PopulateModifies(std::string stmt, std::string var) {
  pkb->GetModifyStore()->AddStmtVar(stmt, var);
}

void Populator::PopulateModifiesProc(std::string proc, std::string var) {
  pkb->GetModifyStore()->AddProcVar(proc, var);
}

void Populator::PopulateProc(std::string name) {
  pkb->AddStmt(name, PROC);
}

void Populator::PopulateAssign(std::string stmt) {
  pkb->AddStmt(stmt, ASSIGN);
}

void Populator::PopulateStmt(std::string stmt) {
  pkb->AddStmt(stmt, STMT);
}

void Populator::PopulateRead(std::string stmt) {
  pkb->AddStmt(stmt, READ);
}

void Populator::PopulatePrint(std::string stmt) {
  pkb->AddStmt(stmt, PRINT);
}

void Populator::PopulateVars(std::string var) {
  pkb->AddStmt(var, VARS);
}

void Populator::PopulateWhile(std::string stmt) {
  pkb->AddStmt(stmt, WHILE);
}

void Populator::PopulateIf(std::string stmt) {
  pkb->AddStmt(stmt, IF);
}

void Populator::PopulateConst(std::string name) {
  pkb->AddStmt(name, CONSTS);
}

void Populator::AddPattern(StmtType type, std::string stmt, std::string lhs, std::string rhs) {
  if (type == StmtType::STMT) {
    pkb->GetPatternStore()->AddStmtWithPattern(stmt, lhs, rhs);
  } else if (type == StmtType::WHILE) {
    pkb->GetPatternStore()->AddWhileWithPattern(stmt, lhs);
  } else if (type == StmtType::IF) {
    pkb->GetPatternStore()->AddIfWithPattern(stmt, lhs);
  }
}

void Populator::PopulateCfg(source::CfgProgramNode &cfg) {
  pkb->AddProgramCfg(std::make_shared<source::CfgProgramNode>(cfg));
}
