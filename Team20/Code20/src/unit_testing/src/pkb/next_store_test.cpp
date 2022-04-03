#include "components/pkb/stores/next_store/next_store.h"
#include "next_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"
#include "components/pkb/pkb.h"

std::shared_ptr<NextStore> set_up_next() {
  PKB *pkb = new PKB();
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
  pkb->AddTypeOfStmt("1", STMT);
  pkb->AddTypeOfStmt("2", STMT);
  pkb->AddTypeOfStmt("3", STMT);
  pkb->AddTypeOfStmt("4", STMT);
  pkb->AddTypeOfStmt("5", STMT);
  pkb->AddTypeOfStmt("6", STMT);
  pkb->AddTypeOfStmt("7", STMT);
  pkb->AddTypeOfStmt("8", STMT);
  pkb->AddTypeOfStmt("9", STMT);
  pkb->AddTypeOfStmt("10", STMT);
  pkb->AddTypeOfStmt("11", STMT);
  pkb->AddTypeOfStmt("12", STMT);
  pkb->AddTypeOfStmt("13", STMT);
  pkb->AddTypeOfStmt("14", STMT);
  pkb->AddTypeOfStmt("15", STMT);

  pkb->GetNextStore()->AddNext(LINE_1, LINE_8);
  pkb->GetNextStore()->AddNext(LINE_1, LINE_2);
  pkb->GetNextStore()->AddNext(LINE_2, LINE_7);
  pkb->GetNextStore()->AddNext(LINE_2, LINE_3);
  pkb->GetNextStore()->AddNext(LINE_3, LINE_5);
  pkb->GetNextStore()->AddNext(LINE_3, LINE_4);
  pkb->GetNextStore()->AddNext(LINE_5, LINE_6);
  pkb->GetNextStore()->AddNext(LINE_6, LINE_5);
  pkb->GetNextStore()->AddNext(LINE_9, LINE_10);
  pkb->GetNextStore()->AddNext(LINE_9, LINE_11);
  pkb->GetNextStore()->AddNext(LINE_10, LINE_15);
  pkb->GetNextStore()->AddNext(LINE_11, LINE_12);
  pkb->GetNextStore()->AddNext(LINE_12, LINE_13);
  pkb->GetNextStore()->AddNext(LINE_12, LINE_14);
  pkb->GetNextStore()->AddNext(LINE_13, LINE_12);
  pkb->GetNextStore()->AddNext(LINE_14, LINE_15);
  pkb->GetNextStore()->AddNext(LINE_15, LINE_8);
  return pkb->GetNextStore();
}

TEST_CASE("Valid next pair") {
  std::shared_ptr<NextStore> next_store = set_up_next();
  std::string stmt1 = "9";
  std::string stmt2 = "11";
  bool actual = next_store->IsNextValid({stmt1, stmt2});
  bool expected = all_next_pairs.find({stmt1, stmt2}) != all_next_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid next pair") {
  std::shared_ptr<NextStore> next_store = set_up_next();
  std::string stmt1 = "4";
  std::string stmt2 = "5";
  bool actual = next_store->IsNextValid({stmt1, stmt2});
  bool expected = all_next_pairs.find({stmt1, stmt2}) != all_next_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Valid next list") {
  std::shared_ptr<NextStore> next_store = set_up_next();
  std::string stmt1 = "3";
  std::unordered_set<std::string> actual;
  actual.insert("4");
  actual.insert("5");
  std::unordered_set<std::string> expected = next_store->GetNextOf(STMT, stmt1);

  REQUIRE(actual == expected);
}

TEST_CASE("Valid before of stmt") {
  std::shared_ptr<NextStore> next_store = set_up_next();
  std::string stmt1 = "15";
  std::unordered_set<std::string> actual = before_map.at(stmt1);
  std::unordered_set<std::string> expected = next_store->GetBeforeOf(stmt1);

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid before of stmt") {
  std::shared_ptr<NextStore> next_store = set_up_next();
  std::string stmt1 = "15";
  std::unordered_set<std::string> actual;
  actual.insert("9");
  actual.insert("10");
  actual.insert("14");
  std::unordered_set<std::string> expected = next_store->GetBeforeOf(stmt1);

  REQUIRE(actual != expected);
}

