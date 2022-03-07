#include "populator.h"
#include "components/pkb/pkb.h"

Populator::Populator(PKB *pkb) :
    pkb(pkb) {
  this->pkb = pkb;
}

void Populator::PopulateParent(std::string stmt1, std::string stmt2) {
  this->pkb->AddParentStmt(stmt1, stmt2);
}

void Populator::PopulateParentStar(std::string stmt, std::vector<std::string> visited) {
  this->pkb->AddParentStarStmt(stmt, visited);
}

void Populator::PopulateFollows(std::string stmt1, std::string stmt2) {
  this->pkb->AddFollowStmt(stmt1, stmt2);
}

void Populator::PopulateFollowsStar(std::vector<std::shared_ptr<StatementNode>> stmt_lst, int index) {
  int curr_stmt = stmt_lst[index]->GetStatementNumber();

  for (int i = index + 1; i < stmt_lst.size(); ++i) {
    int next_stmt = stmt_lst[i]->GetStatementNumber();
    this->pkb->AddFollowStarStmt(std::to_string(curr_stmt), std::to_string(next_stmt));
  }
}

void Populator::PopulateUses(std::string stmt, std::string var) {
  this->pkb->AddUsageStmtVar(stmt, var);
}

void Populator::PopulateModifies(std::string stmt, std::string var) {
  this->pkb->AddModifyStmtVar(stmt, var);
}

void Populator::PopulateProc(std::string name) {
  this->pkb->AddStmt(name, PROC);
}

void Populator::PopulateAssign(std::string stmt) {
  this->pkb->AddStmt(stmt, ASSIGN);
}

void Populator::PopulateStmt(std::string stmt) {
  this->pkb->AddStmt(stmt, STMT);
}

void Populator::PopulateRead(std::string stmt) {
  this->pkb->AddStmt(stmt, READ);
}

void Populator::PopulatePrint(std::string stmt) {
  this->pkb->AddStmt(stmt, PRINT);
}

void Populator::PopulateVars(std::string var) {
  this->pkb->AddStmt(var, VARS);
}

void Populator::PopulateWhile(std::string stmt) {
  this->pkb->AddStmt(stmt, WHILE);
}

void Populator::PopulateIf(std::string stmt) {
  this->pkb->AddStmt(stmt, IF);
}

void Populator::PopulateConst(std::string name) {
  this->pkb->AddStmt(name, CONSTS);
}

void Populator::AddPattern(std::string stmt, std::string lhs, std::string rhs) {
  this->pkb->AddPattern(stmt, lhs, rhs);
}