#include "populator.h"

Populator::Populator(PKB *pkb) : pkb(pkb) {}

void Populator::PopulateParent(std::string stmt1, std::string stmt2) {
  pkb->AddParentStmt(stmt1, stmt2);
}

void Populator::PopulateParentStar(std::string stmt, std::vector<std::string> visited) {
  pkb->AddParentStarStmt(stmt, visited);
}

void Populator::PopulateFollows(std::string stmt1, std::string stmt2) {
  pkb->AddFollowStmt(stmt1, stmt2);
}

void Populator::PopulateFollowsStar(std::string stmt1, std::string stmt2) {
  pkb->AddFollowStarStmt(stmt1, stmt2);
}

void Populator::PopulateUses(std::string stmt, std::string var) {
  pkb->AddUsageStmtVar(stmt, var);
}

void Populator::PopulateModifies(std::string stmt, std::string var) {
  pkb->AddModifyStmtVar(stmt, var);
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

void Populator::AddPattern(std::string stmt, std::string lhs, std::string rhs) {
  pkb->AddPattern(stmt, lhs, rhs);
}
