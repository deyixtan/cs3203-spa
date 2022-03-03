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
  pkb->AddUsageStmtVar("4", "dragon");
  pkb->AddUsageStmtVar("4", "rabbit");
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

  pkb->AddModifyProcVar("main", "dog");
  pkb->AddModifyProcVar("main", "pig");
  pkb->AddModifyProcVar("main", "dragon");
  pkb->AddModifyProcVar("foo", "snake");
  pkb->AddModifyProcVar("func", "monkey");

  pkb->AddModifyStmtVar("1", "dog");
  pkb->AddModifyStmtVar("4", "dog");
  pkb->AddModifyStmtVar("7", "pig");
  pkb->AddModifyStmtVar("8", "dragon");
  pkb->AddModifyStmtVar("10", "snake");
  pkb->AddModifyStmtVar("15", "monkey");

  pkb->AddFollowStmt("1", "2");
  pkb->AddFollowStmt("2", "3");
  pkb->AddFollowStmt("3", "4");
  pkb->AddFollowStmt("4", "5");
  pkb->AddFollowStmt("8", "9");
  pkb->AddFollowStmt("10", "11");
  pkb->AddFollowStmt("13", "14");

  pkb->AddFollowStarStmt("1", "2");
  pkb->AddFollowStarStmt("1", "3");
  pkb->AddFollowStarStmt("1", "4");
  pkb->AddFollowStarStmt("1", "5");
  pkb->AddFollowStarStmt("2", "3"),
  pkb->AddFollowStarStmt("2", "4"),
  pkb->AddFollowStarStmt("2", "5"),
  pkb->AddFollowStarStmt("3", "4"),
  pkb->AddFollowStarStmt("3", "5"),
  pkb->AddFollowStarStmt("4", "5"),
  pkb->AddFollowStarStmt("8", "9"),
  pkb->AddFollowStarStmt("8", "9");
  pkb->AddFollowStarStmt("10", "11");
  pkb->AddFollowStarStmt("13", "14");

  pkb->AddParentStmt("5", "6");
  pkb->AddParentStmt("5", "8");
  pkb->AddParentStmt("5", "9");
  pkb->AddParentStmt("6", "7");
  pkb->AddParentStmt("11", "12");

  pkb->AddPattern("4", "dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))");
  pkb->AddPattern("7", "pig", "(ox+cat)");
  pkb->AddPattern("8", "dragon", "((dog*rabbit)/mouse)");
  pkb->AddPattern("10", "snake", "(dog+rabbit)");
  pkb->AddPattern("15", "monkey", "(tiger+dog)");
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

/* USAGE STORE */

TEST_CASE("Get var used by stmt (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarUsedByStmt("7");
  auto expected = uses_stmt_to_var.at("7");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by stmt (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarUsedByStmt("20");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt used by var (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmtUsedByVar("rabbit");
  auto expected = uses_var_to_stmt.at("rabbit");

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt used by var (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmtUsedByVar("horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by proc (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarUsedByProc("main");
  auto expected = uses_proc_to_var.at("main");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var used by proc (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarUsedByProc("foobar");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc used by var (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetProcUsedByVar("dog");
  auto expected = uses_var_to_proc.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc used by var (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetProcUsedByVar("horse");
  std::unordered_set<std::string> expected = {};

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

TEST_CASE("Get all stmt using (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllStmtUsing();
  auto expected = all_stmt_using;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all proc using (correct)") {
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

/* MODIFY STORE */

TEST_CASE("Get var modified by stmt (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarModByStmt("7");
  auto expected = mod_stmt_to_var.at("7");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var modified by stmt (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarModByStmt("20");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt modified by var (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmtModByVar("dog");
  auto expected = mod_var_to_stmt.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get stmt modified by var (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetStmtModByVar("horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get var modified by proc (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarModByProc("main");
  auto expected = mod_proc_to_var.at("main");

  REQUIRE(actual == expected);
}

TEST_CASE("Get var modified by proc (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetVarModByProc("foobar");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc modified by var (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetProcModByVar("dog");
  auto expected = mod_var_to_proc.at("dog");

  REQUIRE(actual == expected);
}

TEST_CASE("Get proc modified by var (invalid)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetProcModByVar("horse");
  std::unordered_set<std::string> expected = {};

  REQUIRE(actual == expected);
}

TEST_CASE("Get all modifies proc-var pairs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllModProcVar();
  auto expected = mod_proc_var_pairs;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all modifies stmt-var pairs") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllModStmtVar();
  auto expected = mod_stmt_var_pairs;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all stmt modifies (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllStmtModify();
  auto expected = all_stmt_mod;

  REQUIRE(actual == expected);
}

TEST_CASE("Get all proc modifies (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetAllProcModify();
  auto expected = all_proc_mod;

  REQUIRE(actual == expected);
}

TEST_CASE("Check if modifies proc-var pair exists (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsModifyProcVarExist({"main", "dragon"});
  auto expected = mod_proc_var_pairs.find({"main", "dragon"}) != mod_proc_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if modifies proc-var pair exists (wrong)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsModifyProcVarExist({"funcA", "2"});
  auto expected = mod_proc_var_pairs.find({"funcA", "2"}) != mod_proc_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

TEST_CASE("Check if modifies stmt-var pair exists (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsModifyStmtVarExist({"15", "monkey"});
  auto expected = mod_stmt_var_pairs.find({"15", "monkey"}) != mod_stmt_var_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if modifies stmt-var pair exists (wrong)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsModifyStmtVarExist({"5", "cat"});
  auto expected = mod_stmt_var_pairs.find({"5", "cat"}) != mod_stmt_var_pairs.end();

  REQUIRE(actual == false);
  REQUIRE(expected == false);
}

/* FOLLOW STORE */

TEST_CASE("Check if stmt is a follower") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsFollower("4");
  auto expected = followers.find("4") != followers.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if stmt is a following") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsFollowing("4");
  auto expected = followings.find("4") != followings.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if stmt is a follower star") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsFollowerStar("4");
  auto expected = follower_stars.find("4") != follower_stars.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if stmt is a following star") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsFollowingStar("4");
  auto expected = following_stars.find("4") != following_stars.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if follow pair exists") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsFollowExist({"8", "9"});
  auto expected = follow_pairs.find({"8", "9"}) != follow_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Check if follow star pair exists") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsFollowStarExist({"1", "4"});
  auto expected = follow_star_pairs.find({"1", "4"}) != follow_star_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Get follows of a stmt") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetFollowingOf("2");
  auto expected = follows_rs.at("2").following;

  REQUIRE(actual == expected);
}

TEST_CASE("Get follows star of a stmt") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->GetFollowingStarOf("2");
  auto expected = follows_rs.at("2").following_star;

  REQUIRE(actual == expected);
}

//TEST_CASE("Get all follow statements") {
//  set_up_pkb();
//  PKB *pkb = PKB::GetInstance();
//  auto actual = pkb->GetAllFollowStmt(STMT);
//  auto expected = mod_stmt_var_pairs.find({"5", "cat"}) != mod_stmt_var_pairs.end();
//
//  REQUIRE(actual == false);
//  REQUIRE(expected == false);
//}
//
//TEST_CASE("Get all follow star statements") {
//  set_up_pkb();
//  PKB *pkb = PKB::GetInstance();
//  auto actual = pkb->GetAllFollowStarStmt(STMT);
//  auto expected = mod_stmt_var_pairs.find({"5", "cat"}) != mod_stmt_var_pairs.end();
//
//  REQUIRE(actual == false);
//  REQUIRE(expected == false);
//}

/* PARENT STORE */

TEST_CASE("Checks if a stmt is a parent") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsParent("5");
  auto expected = parents.find("5") != parents.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Checks if a stmt is a child") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->IsChild("7");
  auto expected = children.find("7") != children.end();

  REQUIRE(actual == expected);
}

//TEST_CASE("Checks if a stmt is an ancestor") {
//  set_up_pkb();
//  PKB *pkb = PKB::GetInstance();
//  auto actual = pkb->IsAnce("5");
//  auto expected = ancestors.find("5") != ancestors.end();
//
//  REQUIRE(actual == expected);
//}

//TEST_CASE("Checks if a stmt is a descendent") {
//  set_up_pkb();
//  PKB *pkb = PKB::GetInstance();
//  auto actual = pkb->IsDesc("7");
//  auto expected = descendants.find("7") != descendants.end();
//
//  REQUIRE(actual == expected);
//}

TEST_CASE("Checks if a parent-child pair relationship exists") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  auto actual = pkb->ParentChildExists("5", "9");
  auto expected = parent_child_pairs.find({"5", "9"}) != parent_child_pairs.end();

  REQUIRE(actual == expected);
}

//TEST_CASE("Checks if a ance-desc pair relationship exists") {
//  set_up_pkb();
//  PKB *pkb = PKB::GetInstance();
//  auto actual = pkb->AnceExists("5", "9");
//  auto expected = ance_desc_pairs.find({"5", "9"}) != ance_desc_pairs.end();
//
//  REQUIRE(actual == expected);
//}

/* GENERAL */

void Init(int num_stmts);

void AddFollow(std::string parent, std::string child);

void AddFollowStar(std::string parent, std::string child);

std::unordered_set<std::string> GetAllFollowers();

std::string GetFollowerOf(std::string stmt);

std::string GetFollowingOf(std::string stmt);

std::unordered_set<std::string> GetFollowerStarOf(std::string stmt);

std::unordered_set<std::string> GetFollowingStarOf(std::string stmt);

std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowPairs();

std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetFollowStarPairs();


/* PARENT STORE */



/* PATTERN */

TEST_CASE("Check pattern matching (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  std::unordered_set<std::string> actual = pkb->GetStmtWithPattern("pig", "_\"cat\"_");
  std::unordered_set<std::string> expected = {};
  for (auto const& [key, val] : pattern_to_stmt) {
    if (key.first == "pig" && key.second.find("cat") != -1) {
      expected.insert(val);
    }
  }

  REQUIRE(actual == expected);
}

TEST_CASE("Check pattern matching (wrong)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  std::unordered_set<std::string> actual = pkb->GetStmtWithPattern("dog", "rabbit - cat");
  std::unordered_set<std::string> expected = {};
  for (auto const& [key, val] : pattern_to_stmt) {
    if (key.first == "dog" && key.second.find("(rabbit-cat)") != -1) {
      expected.insert(val);
    }
  }

  REQUIRE(actual == std::unordered_set<std::string>());
  REQUIRE(expected == std::unordered_set<std::string>());
}

TEST_CASE("Check pattern synonym matching exact (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> actual = pkb->GetStmtWithPatternSynonym("dog");
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> expected;
  for (auto i : pattern_pairs) {
    if (i.second == "dog") {
      expected.insert({pattern_to_stmt.at(i), i.first});
    }
  }

  REQUIRE(actual == expected);
}

TEST_CASE("Check pattern synonym matching partial (correct)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> actual = pkb->GetStmtWithPatternSynonym("_\"dog\"_");
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> expected;
  for (auto i : pattern_pairs) {
    if (i.second.find("dog") != std::string::npos) {
      expected.insert({pattern_to_stmt.at(i), i.first});
    }
  }

  REQUIRE(actual == expected);
}