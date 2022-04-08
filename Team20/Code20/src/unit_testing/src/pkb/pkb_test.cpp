#include "pkb_stub.h"
#include "components/pkb/pkb.h"
#include "catch.hpp"

/*
	procedure main {
1		read dog;
2		call foo;
3		print cat;
4		dog = mouse + 10 * cat - (dog / mouse) * dragon + mouse + rabbit - cat;
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

PKB *set_up_pkb() {
  PKB *pkb = new PKB();
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

  pkb->AddTypeOfStmt("1", READ);
  pkb->AddTypeOfStmt("2", STMT);
  pkb->AddTypeOfStmt("3", PRINT);
  pkb->AddTypeOfStmt("4", ASSIGN);
  pkb->AddTypeOfStmt("5", STMT);
  pkb->AddTypeOfStmt("6", STMT);
  pkb->AddTypeOfStmt("7", ASSIGN);
  pkb->AddTypeOfStmt("8", ASSIGN);
  pkb->AddTypeOfStmt("9", STMT);
  pkb->AddTypeOfStmt("10", ASSIGN);
  pkb->AddTypeOfStmt("11", STMT);
  pkb->AddTypeOfStmt("12", STMT);
  pkb->AddTypeOfStmt("13", PRINT);
  pkb->AddTypeOfStmt("14", STMT);
  pkb->AddTypeOfStmt("15", ASSIGN);

  pkb->GetUsesStore()->AddStmtVar("3", "cat");
  pkb->GetUsesStore()->AddStmtVar("4", "dog");
  pkb->GetUsesStore()->AddStmtVar("4", "mouse");
  pkb->GetUsesStore()->AddStmtVar("4", "cat");
  pkb->GetUsesStore()->AddStmtVar("4", "dragon");
  pkb->GetUsesStore()->AddStmtVar("4", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("5", "dog");
  pkb->GetUsesStore()->AddStmtVar("6", "pig");
  pkb->GetUsesStore()->AddStmtVar("6", "mouse");
  pkb->GetUsesStore()->AddStmtVar("7", "pig");
  pkb->GetUsesStore()->AddStmtVar("7", "ox");
  pkb->GetUsesStore()->AddStmtVar("7", "cat");
  pkb->GetUsesStore()->AddStmtVar("8", "dragon");
  pkb->GetUsesStore()->AddStmtVar("8", "dog");
  pkb->GetUsesStore()->AddStmtVar("8", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("8", "mouse");
  pkb->GetUsesStore()->AddStmtVar("10", "snake");
  pkb->GetUsesStore()->AddStmtVar("10", "dog");
  pkb->GetUsesStore()->AddStmtVar("10", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("11", "cat");
  pkb->GetUsesStore()->AddStmtVar("13", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("15", "monkey");
  pkb->GetUsesStore()->AddStmtVar("15", "tiger");
  pkb->GetUsesStore()->AddStmtVar("15", "dog");
  pkb->GetUsesStore()->AddStmtVar("main", "cat");
  pkb->GetUsesStore()->AddStmtVar("main", "dog");
  pkb->GetUsesStore()->AddStmtVar("main", "mouse");
  pkb->GetUsesStore()->AddStmtVar("main", "dragon");
  pkb->GetUsesStore()->AddStmtVar("main", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("main", "pig");
  pkb->GetUsesStore()->AddStmtVar("main", "ox");
  pkb->GetUsesStore()->AddStmtVar("foo", "cat");
  pkb->GetUsesStore()->AddStmtVar("foo", "dog");
  pkb->GetUsesStore()->AddStmtVar("foo", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("foo", "monkey");
  pkb->GetUsesStore()->AddStmtVar("foo", "tiger");
  pkb->GetUsesStore()->AddStmtVar("bar", "dog");
  pkb->GetUsesStore()->AddStmtVar("bar", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("bar", "tiger");
  pkb->GetUsesStore()->AddStmtVar("func", "tiger");
  pkb->GetUsesStore()->AddStmtVar("func", "dog");

  pkb->GetModifiesStore()->AddStmtVar("1", "dog");
  pkb->GetModifiesStore()->AddStmtVar("4", "dog");
  pkb->GetModifiesStore()->AddStmtVar("7", "pig");
  pkb->GetModifiesStore()->AddStmtVar("8", "dragon");
  pkb->GetModifiesStore()->AddStmtVar("10", "snake");
  pkb->GetModifiesStore()->AddStmtVar("15", "monkey");
  pkb->GetModifiesStore()->AddStmtVar("main", "dog");
  pkb->GetModifiesStore()->AddStmtVar("main", "pig");
  pkb->GetModifiesStore()->AddStmtVar("main", "dragon");
  pkb->GetModifiesStore()->AddStmtVar("main", "snake");
  pkb->GetModifiesStore()->AddStmtVar("main", "monkey");
  pkb->GetModifiesStore()->AddStmtVar("foo", "snake");
  pkb->GetModifiesStore()->AddStmtVar("foo", "monkey");
  pkb->GetModifiesStore()->AddStmtVar("func", "monkey");
  pkb->GetModifiesStore()->AddStmtVar("bar", "monkey");

  pkb->GetFollowsStore()->AddFollow("1", "2");
  pkb->GetFollowsStore()->AddFollow("2", "3");
  pkb->GetFollowsStore()->AddFollow("3", "4");
  pkb->GetFollowsStore()->AddFollow("4", "5");
  pkb->GetFollowsStore()->AddFollow("8", "9");
  pkb->GetFollowsStore()->AddFollow("10", "11");
  pkb->GetFollowsStore()->AddFollow("13", "14");

  pkb->GetFollowsStore()->AddFollowStar("1", "2");
  pkb->GetFollowsStore()->AddFollowStar("1", "3");
  pkb->GetFollowsStore()->AddFollowStar("1", "4");
  pkb->GetFollowsStore()->AddFollowStar("1", "5");
  pkb->GetFollowsStore()->AddFollowStar("2", "3");
  pkb->GetFollowsStore()->AddFollowStar("2", "4");
  pkb->GetFollowsStore()->AddFollowStar("2", "5");
  pkb->GetFollowsStore()->AddFollowStar("3", "4");
  pkb->GetFollowsStore()->AddFollowStar("3", "5");
  pkb->GetFollowsStore()->AddFollowStar("4", "5");
  pkb->GetFollowsStore()->AddFollowStar("8", "9");
  pkb->GetFollowsStore()->AddFollowStar("8", "9");
  pkb->GetFollowsStore()->AddFollowStar("10", "11");
  pkb->GetFollowsStore()->AddFollowStar("13", "14");

  pkb->GetParentStore()->AddParent("5", "6");
  pkb->GetParentStore()->AddParent("5", "8");
  pkb->GetParentStore()->AddParent("5", "9");
  pkb->GetParentStore()->AddParent("6", "7");
  pkb->GetParentStore()->AddParent("11", "12");

  pkb->GetPatternStore()->AddStmtWithPattern("4", "dog", "((((mouse)+((10)*(cat)))-(((dog)/(mouse))*(dragon)))+(((mouse)+(rabbit))-(cat)))");
  pkb->GetPatternStore()->AddStmtWithPattern("7", "pig", "((ox)+(cat))");
  pkb->GetPatternStore()->AddStmtWithPattern("8", "dragon", "(((dog)*(rabbit))/(mouse))");
  pkb->GetPatternStore()->AddStmtWithPattern("10", "snake", "((dog)+(rabbit))");
  pkb->GetPatternStore()->AddStmtWithPattern("15", "monkey", "((tiger)+(dog))");

  pkb->AddStmtToName(PRINT, "3", "cat");
  pkb->AddNameToStmt(PRINT, "cat", "3");
  pkb->AddStmtToName(PRINT, "13", "rabbit");
  pkb->AddNameToStmt(PRINT, "rabbit", "13");
  pkb->AddStmtToName(READ, "1", "dog");
  pkb->AddNameToStmt(READ, "dog", "1");
  return pkb;
}

TEST_CASE("PKB instance") {
  PKB *pkb1 = new PKB();
  PKB *pkb2 = new PKB();

  REQUIRE(pkb1 != pkb2);
}

TEST_CASE("proc p; select p") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(PROC);
  auto expected = proc_list;

  REQUIRE(actual == expected);
}

TEST_CASE("stmt s; select s") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(STMT);
  auto expected = stmt_list;

  REQUIRE(actual == expected);
}

TEST_CASE("variable v; select v") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(VARS);
  auto expected = var_list;

  REQUIRE(actual == expected);
}

TEST_CASE("assign a; select a") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(ASSIGN);
  auto expected = assign_list;

  REQUIRE(actual == expected);
}

TEST_CASE("if ifs; select ifs") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(IF);
  auto expected = if_list;

  REQUIRE(actual == expected);
}

TEST_CASE("while w; select w") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(WHILE);
  auto expected = while_list;

  REQUIRE(actual == expected);
}

TEST_CASE("const c; select c") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(CONSTS);
  auto expected = const_list;

  REQUIRE(actual == expected);
}

TEST_CASE("read r; select r") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(READ);
  auto expected = read_list;

  REQUIRE(actual == expected);
}

TEST_CASE("print p; select p") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetStmt(PRINT);
  auto expected = print_list;

  REQUIRE(actual == expected);
}

/* USAGE STORE */

TEST_CASE("Get var used by stmt (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetVarUsedByStmt("7");
  auto expected = uses_stmt_to_var.at("7");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by stmt (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetVarUsedByStmt("20");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt used by var (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "rabbit");
  auto expected = uses_var_to_stmt.at("rabbit");

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt used by var (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by proc (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetVarUsedByStmt("main");
  auto expected = uses_proc_to_var.at("main");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by proc (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetVarUsedByStmt("foobar");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc used by var (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "dog");
  auto expected = uses_var_to_stmt.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc used by var (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get all stmt using (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->GetAllStmtUsing();
  auto expected = all_stmt_using;

  REQUIRE(actual == expected);
}

TEST_CASE("Check if usage proc-var pair Valid (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->IsStmtVarValid({"funcX", "c"});
  auto expected = uses_proc_var_pairs.find({"funcX", "c"}) != uses_proc_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if usage proc-var pair Valid (wrong)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->IsStmtVarValid({"funcA", "2"});
  auto expected = uses_proc_var_pairs.find({"funcA", "2"}) != uses_proc_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

TEST_CASE("Check if usage stmt-var pair Valid (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->IsStmtVarValid({"4", "k"});
  auto expected = uses_stmt_var_pairs.find({"4", "k"}) != uses_stmt_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if usage stmt-var pair Valid (wrong)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetUsesStore()->IsStmtVarValid({"5", "j"});
  auto expected = uses_stmt_var_pairs.find({"5", "j"}) != uses_stmt_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

/* MODIFY STORE */

TEST_CASE("Get var modified by stmt (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetVarModByStmt("7");
  auto expected = mod_stmt_to_var.at("7");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var modified by stmt (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetVarModByStmt("20");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt modified by var (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "dog");
  auto expected = mod_var_to_stmt.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt modified by var (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get var modified by proc (correct)") {
  PKB *pkb = set_up_pkb();

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> actual = pkb->GetModifiesStore()->GetAllModStmt(PROC);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> expected = mod_proc_to_var.at(PROC);

  REQUIRE(actual == expected);
}

TEST_CASE("Get var modified by proc (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetVarModByStmt("foobar");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc modified by var (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetStmtModByVar(PROC, "dog");
  auto expected = mod_var_to_proc.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc modified by var (invalid)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetStmtModByVar(PROC, "horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get all stmt modifies (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetAllStmtModify();
  auto expected = all_stmt_mod;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all proc modifies (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->GetAllProcModify();
  auto expected = all_proc_mod;

  REQUIRE(actual == expected);
}

TEST_CASE("Check if modifies proc-var pair Valid (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->IsStmtVarValid({"main", "dragon"});
  auto expected = mod_proc_var_pairs.find({"main", "dragon"}) != mod_proc_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if modifies proc-var pair Valid (wrong)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->IsStmtVarValid({"funcA", "2"});
  auto expected = mod_proc_var_pairs.find({"funcA", "2"}) != mod_proc_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

TEST_CASE("Check if modifies stmt-var pair Valid (correct)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->IsStmtVarValid({"15", "monkey"});
  auto expected = mod_stmt_var_pairs.find({"15", "monkey"}) != mod_stmt_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if modifies stmt-var pair Valid (wrong)") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetModifiesStore()->IsStmtVarValid({"5", "cat"});
  auto expected = mod_stmt_var_pairs.find({"5", "cat"}) != mod_stmt_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

/* FOLLOW STORE */
TEST_CASE("Check if follow pair Valid") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetFollowsStore()->IsFollowsPairValid({"8", "9"});
  auto expected = follow_pairs.find({"8", "9"}) != follow_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if follow star pair Valid") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetFollowsStore()->IsFollowsStarPairValid({"1", "4"});
  auto expected = follow_star_pairs.find({"1", "4"}) != follow_star_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Get follows of a stmt") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetFollowsStore()->GetFollowingOf(STMT, "2");
  auto expected = follows_rs.at("2").following;

  REQUIRE(actual == expected);
}

TEST_CASE("Get follows star of a stmt") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "2");
  auto expected = follows_rs.at("2").following_star;

  REQUIRE(actual == expected);
}

/* PARENT STORE */
TEST_CASE("Checks if a parent-child pair relationship Valid") {
  PKB *pkb = set_up_pkb();
  
  auto actual = pkb->GetParentStore()->IsParentPairValid({"5", "9"});
  auto expected = parent_child_pairs.find({"5", "9"}) != parent_child_pairs.end();

  REQUIRE(actual == expected);
}

/* GENERAL */

void Init(int num_stmts);

void AddFollow(std::string parent, std::string child);

void AddFollowStar(std::string parent, std::string child);

std::unordered_set<std::string> GetAllFollowers();

std::string GetFollowerOf(std::string stmt);

std::string GetFollowingOf(std::string stmt);

std::unordered_set<std::string> GetFollowerStarOf(std::string stmt);

std::unordered_set<std::string> GetFollowingStarOf(std::string stmt);


/* PARENT STORE */



/* PATTERN */

TEST_CASE("Check pattern matching (correct)") {
  PKB *pkb = set_up_pkb();
  
  std::unordered_set<std::string> actual = pkb->GetPatternStore()->GetStmtWithPatternPartial("pig", "cat");
  std::unordered_set<std::string> expected = {};
  for (auto const&[key, val] : pattern_to_stmt) {
    if (key.first == "pig" && key.second.find("(cat)") != -1) {
      expected.insert(val);
    }
  }

  REQUIRE(actual == expected);
}

TEST_CASE("Check pattern matching (wrong)") {
  PKB *pkb = set_up_pkb();
  
  std::unordered_set<std::string> actual = pkb->GetPatternStore()->GetStmtWithPatternExact("dog", "rabbit - cat");
  std::unordered_set<std::string> expected = {};
  for (auto const&[key, val] : pattern_to_stmt) {
    if (key.first == "dog" && key.second.find("((rabbit)-(cat))") != -1) {
      expected.insert(val);
    }
  }

  REQUIRE(actual == std::unordered_set<std::string>());
  REQUIRE(expected == std::unordered_set<std::string>());
}

TEST_CASE("Check pattern synonym matching exact (correct)") {
  PKB *pkb = set_up_pkb();
  
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> actual = pkb->GetPatternStore()->GetStmtWithPatternSynonymWildcard();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> expected = pattern_pairs_synonym;
  REQUIRE(actual == expected);
}

TEST_CASE("Check pattern synonym matching partial (correct)") {
  PKB *pkb = set_up_pkb();
  
  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      actual = pkb->GetPatternStore()->GetStmtWithPatternSynonymWildcard();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> expected = pattern_pairs_synonym;

  REQUIRE(actual == expected);
}