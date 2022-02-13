#include "pkb_stub.h"
#include "components/pkb/pkb.h"
#include "catch.hpp"

void set_up_pkb() {
  PKB *pkb = PKB::GetInstance();
  pkb->AddStmt("a", VARS);
  pkb->AddStmt("b", VARS);
  pkb->AddStmt("c", VARS);
  pkb->AddStmt("d", VARS);
  pkb->AddStmt("e", VARS);
  pkb->AddStmt("f", VARS);
  pkb->AddStmt("g", VARS);
  pkb->AddStmt("h", VARS);
  pkb->AddStmt("i", VARS);
  pkb->AddStmt("j", VARS);

  pkb->AddStmt("1", STMT);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("4", STMT);
  pkb->AddStmt("5", STMT);
  pkb->AddStmt("6", STMT);
  pkb->AddStmt("7", STMT);
  pkb->AddStmt("8", STMT);
  pkb->AddStmt("9", STMT);
  pkb->AddStmt("10", STMT);

  pkb->AddStmt("1", ASSIGN);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("3", ASSIGN);
  pkb->AddStmt("4", ASSIGN);
  pkb->AddStmt("5", ASSIGN);
  pkb->AddStmt("6", ASSIGN);
  pkb->AddStmt("7", ASSIGN);
  pkb->AddStmt("8", ASSIGN);
  pkb->AddStmt("9", ASSIGN);
  pkb->AddStmt("10", ASSIGN);
}

TEST_CASE("PKB instance") {
  PKB *pkb1 = PKB::GetInstance();
  PKB *pkb2 = PKB::GetInstance();

  REQUIRE(pkb1 == pkb2);
}

TEST_CASE("stmt s; select s;") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(STMT);
  auto expected = stmt_list;

  REQUIRE(actual == expected);
}

TEST_CASE("variable v; select v;") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(VARS);
  auto expected = var_list;

  REQUIRE(actual == expected);
}

TEST_CASE("assign a; select a;") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(ASSIGN);
  auto expected = assign_stmt_list;

  REQUIRE(actual == expected);
}
