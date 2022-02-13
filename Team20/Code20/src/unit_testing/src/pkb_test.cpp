#include "pkb_stub.h"
#include "components/pkb/pkb.h"
#include "catch.hpp"

void set_up_pkb() {
  PKB *pkb = PKB::GetInstance();
  pkb->AddStmt("main", PROC);
  pkb->AddStmt("foo", PROC);
  pkb->AddStmt("bar", PROC);
  pkb->AddStmt("funcX", PROC);
  pkb->AddStmt("funcY", PROC);
  pkb->AddStmt("funcZ", PROC);

  pkb->AddStmt("4", IF);
  pkb->AddStmt("5", IF);
  pkb->AddStmt("6", IF);

  pkb->AddStmt("7", WHILE);

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
  pkb->AddStmt("k", VARS);
  pkb->AddStmt("l", VARS);
  pkb->AddStmt("m", VARS);
  pkb->AddStmt("n", VARS);
  pkb->AddStmt("o", VARS);
  pkb->AddStmt("p", VARS);
  pkb->AddStmt("q", VARS);
  pkb->AddStmt("r", VARS);
  pkb->AddStmt("s", VARS);
  pkb->AddStmt("t", VARS);
  pkb->AddStmt("u", VARS);
  pkb->AddStmt("v", VARS);

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
  pkb->AddStmt("11", STMT);
  pkb->AddStmt("12", STMT);
  pkb->AddStmt("13", STMT);
  pkb->AddStmt("14", STMT);
  pkb->AddStmt("15", STMT);

  pkb->AddStmt("1", ASSIGN);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("3", ASSIGN);

  pkb->AddStmt("1", CONSTS);
  pkb->AddStmt("4", CONSTS);
  pkb->AddStmt("7", CONSTS);
  pkb->AddStmt("9", CONSTS);
  pkb->AddStmt("11", CONSTS);

  pkb->AddStmt("8", READ);
  pkb->AddStmt("9", READ);
  pkb->AddStmt("10", READ);
  pkb->AddStmt("11", READ);

  pkb->AddStmt("12", PRINT);
  pkb->AddStmt("13", PRINT);
  pkb->AddStmt("14", PRINT);
  pkb->AddStmt("15", PRINT);

  pkb->AddUsageProcVar("main", "1");
  pkb->AddUsageProcVar("main", "2");
  pkb->AddUsageProcVar("main", "3");
  pkb->AddUsageProcVar("main", "4");
  pkb->AddUsageProcVar("main", "5");
  pkb->AddUsageProcVar("main", "6");
  pkb->AddUsageProcVar("foo", "7");
  pkb->AddUsageProcVar("bar", "8");
  pkb->AddUsageProcVar("bar", "9");
  pkb->AddUsageProcVar("funcX", "10");
  pkb->AddUsageProcVar("funcX", "11");
  pkb->AddUsageProcVar("funcX", "12");
  pkb->AddUsageProcVar("funcY", "13");
  pkb->AddUsageProcVar("funcY", "14");
  pkb->AddUsageProcVar("funcZ", "15");

  pkb->AddUsageStmtVar("1", "a");
  pkb->AddUsageStmtVar("1", "b");
  pkb->AddUsageStmtVar("1", "f");
  pkb->AddUsageStmtVar("1", "i");
  pkb->AddUsageStmtVar("2", "c");
  pkb->AddUsageStmtVar("4", "d");
  pkb->AddUsageStmtVar("4", "k");
  pkb->AddUsageStmtVar("4", "n");
  pkb->AddUsageStmtVar("4", "s");
  pkb->AddUsageStmtVar("5", "e");
  pkb->AddUsageStmtVar("5", "o");
  pkb->AddUsageStmtVar("6", "a");
  pkb->AddUsageStmtVar("6", "f");
  pkb->AddUsageStmtVar("6", "g");
  pkb->AddUsageStmtVar("6", "h");
  pkb->AddUsageStmtVar("6", "i");
  pkb->AddUsageStmtVar("6", "j");
  pkb->AddUsageStmtVar("7", "c");
  pkb->AddUsageStmtVar("7", "k");
  pkb->AddUsageStmtVar("7", "l");
  pkb->AddUsageStmtVar("8", "m");
  pkb->AddUsageStmtVar("8", "p");
  pkb->AddUsageStmtVar("9", "c");
  pkb->AddUsageStmtVar("9", "n");
  pkb->AddUsageStmtVar("10", "o");
  pkb->AddUsageStmtVar("11", "c");
  pkb->AddUsageStmtVar("11", "p");
  pkb->AddUsageStmtVar("11", "q");
  pkb->AddUsageStmtVar("12", "r");
  pkb->AddUsageStmtVar("14", "s");
  pkb->AddUsageStmtVar("14", "t");
  pkb->AddUsageStmtVar("14", "u");
  pkb->AddUsageStmtVar("15", "v");

}

TEST_CASE("PKB instance") {
  PKB *pkb1 = PKB::GetInstance();
  PKB *pkb2 = PKB::GetInstance();

  REQUIRE(pkb1 == pkb2);
}

TEST_CASE("proc p; select p") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(PROC);
  auto expected = proc_list;

  REQUIRE(actual == expected);
}

TEST_CASE("stmt s; select s") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(STMT);
  auto expected = stmt_list;

  REQUIRE(actual == expected);
}

TEST_CASE("variable v; select v") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(VARS);
  auto expected = var_list;

  REQUIRE(actual == expected);
}

TEST_CASE("assign a; select a") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(ASSIGN);
  auto expected = assign_list;

  REQUIRE(actual == expected);
}

TEST_CASE("if ifs; select ifs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(IF);
  auto expected = if_list;

  REQUIRE(actual == expected);
}

TEST_CASE("while w; select w") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(WHILE);
  auto expected = while_list;

  REQUIRE(actual == expected);
}

TEST_CASE("const c; select c") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(CONSTS);
  auto expected = const_list;

  REQUIRE(actual == expected);
}

TEST_CASE("read r; select r") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(READ);
  auto expected = read_list;

  REQUIRE(actual == expected);
}

TEST_CASE("print p; select p") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmt(PRINT);
  auto expected = print_list;

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by stmt") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarUsedByStmt("7");
  auto expected = stmt_to_var.at("7");

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt used by var") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmtUsedByVar("c");
  auto expected = var_to_stmt.at("c");

  REQUIRE(actual == expected);
}

TEST_CASE("Get all usage proc-var pairs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllUsageProcVar();
  auto expected = proc_var_pairs;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all usage stmt-var pairs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllUsageStmtVar();
  auto expected = stmt_var_pairs;

  REQUIRE(actual == expected);
}