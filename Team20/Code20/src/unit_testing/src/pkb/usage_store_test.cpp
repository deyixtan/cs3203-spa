#include "components/pkb/usage_store.h"
#include "usage_store_stub.h"
#include "../test_util.h"

#include "catch.hpp"

UsageStore usage_store;

// to be expanded
void set_up() {
  usage_store.add_stmt_var(LINE_1, VAR_A);
  usage_store.add_stmt_var(LINE_1, VAR_C);
  usage_store.add_stmt_var(LINE_1, VAR_E);
  usage_store.add_stmt_var(LINE_1, VAR_J);

  usage_store.add_stmt_var(LINE_2, VAR_B);
  usage_store.add_stmt_var(LINE_2, VAR_D);

  usage_store.add_stmt_var(LINE_3, VAR_H);

  usage_store.add_stmt_var(LINE_4, VAR_B);
  usage_store.add_stmt_var(LINE_4, VAR_J);

  usage_store.add_stmt_var(LINE_5, VAR_A);
  usage_store.add_stmt_var(LINE_5, VAR_B);
  usage_store.add_stmt_var(LINE_5, VAR_C);
  usage_store.add_stmt_var(LINE_5, VAR_D);
  usage_store.add_stmt_var(LINE_5, VAR_E);
  usage_store.add_stmt_var(LINE_5, VAR_F);
  usage_store.add_stmt_var(LINE_5, VAR_G);

  usage_store.add_stmt_var(LINE_6, VAR_C);
  usage_store.add_stmt_var(LINE_6, VAR_F);
  usage_store.add_stmt_var(LINE_6, VAR_G);
  usage_store.add_stmt_var(LINE_6, VAR_H);

  usage_store.add_stmt_var(LINE_7, VAR_D);
  usage_store.add_stmt_var(LINE_7, VAR_I);

  usage_store.add_stmt_var(LINE_8, VAR_F);
  usage_store.add_stmt_var(LINE_8, VAR_H);
  usage_store.add_stmt_var(LINE_8, VAR_J);

  usage_store.add_stmt_var(LINE_9, VAR_B);
  usage_store.add_stmt_var(LINE_9, VAR_E);
  usage_store.add_stmt_var(LINE_9, VAR_G);

  usage_store.add_stmt_var(LINE_10, VAR_B);
  usage_store.add_stmt_var(LINE_10, VAR_E);

  usage_store.add_proc_var(PROC_FOO, VAR_A);
  usage_store.add_proc_var(PROC_BAR, VAR_A);
}

TEST_CASE("Get variables used by statement (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get variables used by statement (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get statements used by variables (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get statements used by variables (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get variables used by procedures (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get variables used by procedures (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get all statement-variable pairs (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get all statement-variable pairs (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get all procedure-variable pairs (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Get all procedure-variable pairs (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}


// to be implemented
TEST_CASE("Check if statement-variable pair exists (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Check if statement-variable pair exists (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Check if procedure-variable pair exists (Correct)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}

// to be implemented
TEST_CASE("Check if procedure-variable pair exists (Wrong)") {
  set_up();
  int num = 5;
  std::unordered_set<std::string> actual = usage_store.get_var_used_by_stmt(num);
  std::unordered_set<std::string> expected = stmt_var_map.at(num);

  REQUIRE(actual == expected);
}