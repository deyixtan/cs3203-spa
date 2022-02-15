#include "pkb_stub.h"
#include "components/pkb/pkb.h"
#include "catch.hpp"

/*
	procedure main {
1		read dog;
2		call foo;
3		print cat;
4		dog = mouse + 10 * cat;
5		if (dog <= 0) then {
6			while ( pig > mouse ) {
7				pig = ox + cat;
			}
		} else {
8			dragon = dog * rabbit / mouse;
9			call bar;
		}
	}
	procedure foo {
10		snake = dog + rabbit;
11		while (10 > cat) {
12			call bar;
		}
	}
	procedure bar {
13		print rabbit;
14		call func;
	}
	procedure func {
15		monkey = tiger + dog;
	}
*/

void set_up_pkb() {
  PKB *pkb = PKB::GetInstance();
  pkb->AddStmt("main", PROC);
  pkb->AddStmt("foo", PROC);
  pkb->AddStmt("bar", PROC);
  pkb->AddStmt("func", PROC);

  pkb->AddStmt("5", IF);

  pkb->AddStmt("6", WHILE);
  pkb->AddStmt("11", WHILE);

  pkb->AddStmt("dog", VARS);
  pkb->AddStmt("cat", VARS);
  pkb->AddStmt("rabbit", VARS);
  pkb->AddStmt("snake", VARS);
  pkb->AddStmt("tiger", VARS);
  pkb->AddStmt("monkey", VARS);
  pkb->AddStmt("pig", VARS);
  pkb->AddStmt("mouse", VARS);
  pkb->AddStmt("dragon", VARS);
  pkb->AddStmt("ox", VARS);

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

  pkb->AddStmt("4", ASSIGN);
  pkb->AddStmt("7", ASSIGN);
  pkb->AddStmt("8", ASSIGN);
  pkb->AddStmt("10", ASSIGN);
  pkb->AddStmt("15", ASSIGN);

  pkb->AddStmt("0", CONSTS);
  pkb->AddStmt("10", CONSTS);

  pkb->AddStmt("1", READ);

  pkb->AddStmt("3", PRINT);
  pkb->AddStmt("13", PRINT);

  pkb->AddUsageProcVar("main", "dog");
  pkb->AddUsageProcVar("main", "cat");
  pkb->AddUsageProcVar("main", "mouse");
  pkb->AddUsageProcVar("main", "pig");
  pkb->AddUsageProcVar("main", "ox");
  pkb->AddUsageProcVar("main", "dragon");
  pkb->AddUsageProcVar("main", "rabbit");
  pkb->AddUsageProcVar("foo", "snake");
  pkb->AddUsageProcVar("foo", "dog");
  pkb->AddUsageProcVar("foo", "rabbit");
  pkb->AddUsageProcVar("foo", "cat");
  pkb->AddUsageProcVar("bar", "rabbit");
  pkb->AddUsageProcVar("func", "monkey");
  pkb->AddUsageProcVar("func", "tiger");
  pkb->AddUsageProcVar("func", "dog");

  pkb->AddUsageStmtVar("3", "cat");
  pkb->AddUsageStmtVar("4", "dog");
  pkb->AddUsageStmtVar("4", "mouse");
  pkb->AddUsageStmtVar("4", "cat");
  pkb->AddUsageStmtVar("5", "dog");
  pkb->AddUsageStmtVar("6", "pig");
  pkb->AddUsageStmtVar("6", "mouse");
  pkb->AddUsageStmtVar("7", "pig");
  pkb->AddUsageStmtVar("7", "ox");
  pkb->AddUsageStmtVar("7", "cat");
  pkb->AddUsageStmtVar("8", "dragon");
  pkb->AddUsageStmtVar("8", "dog");
  pkb->AddUsageStmtVar("8", "rabbit");
  pkb->AddUsageStmtVar("8", "mouse");
  pkb->AddUsageStmtVar("10", "snake");
  pkb->AddUsageStmtVar("10", "dog");
  pkb->AddUsageStmtVar("10", "rabbit");
  pkb->AddUsageStmtVar("11", "cat");
  pkb->AddUsageStmtVar("13", "rabbit");
  pkb->AddUsageStmtVar("15", "monkey");
  pkb->AddUsageStmtVar("15", "tiger");
  pkb->AddUsageStmtVar("15", "dog");

//  pkb->AddPattern("dog", "mouse+(10*cat)");
//  pkb->AddPattern("pig", "ox+cat");
//  pkb->AddPattern("dragon", "(dog*rabbit)/mouse");
//  pkb->AddPattern("snake", "dog+rabbit");
//  pkb->AddPattern("monkey", "tiger+dog");
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
  auto expected = uses_stmt_to_var.at("7");

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt used by var") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmtUsedByVar("rabbit");
  auto expected = uses_var_to_stmt.at("rabbit");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by proc") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarUsedByProc("main");
  auto expected = uses_proc_to_var.at("main");

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc used by var") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetProcUsedByVar("dog");
  auto expected = uses_var_to_proc.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get all usage proc-var pairs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllUsageProcVar();
  auto expected = uses_proc_var_pairs;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all usage stmt-var pairs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllUsageStmtVar();
  auto expected = uses_stmt_var_pairs;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all stmts using") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllStmtUsing();
  auto expected = all_stmt_using;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all procs using") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllProcUsing();
  auto expected = all_proc_using;

  REQUIRE(actual == expected);
}

TEST_CASE("Check if usage proc-var pair exists (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsUsageProcVarExist({"funcX", "c"});
  auto expected = uses_proc_var_pairs.find({"funcX", "c"}) != uses_proc_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if usage proc-var pair exists (wrong)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsUsageProcVarExist({"funcA", "2"});
  auto expected = uses_proc_var_pairs.find({"funcA", "2"}) != uses_proc_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

TEST_CASE("Check if usage stmt-var pair exists (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsUsageStmtVarExist({"4", "k"});
  auto expected = uses_stmt_var_pairs.find({"4", "k"}) != uses_stmt_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if usage stmt-var pair exists (wrong)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsUsageStmtVarExist({"5", "j"});
  auto expected = uses_stmt_var_pairs.find({"5", "j"}) != uses_stmt_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}
