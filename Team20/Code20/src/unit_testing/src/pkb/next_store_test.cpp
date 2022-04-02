#include "components/pkb/stores/next_store/next_store.h"
#include "next_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"

NextStore set_up_next() {
  std::vector<std::unordered_set<std::string>> stmt_vector;
  for (int i = 0; i < COUNT; i++) {
    stmt_vector.push_back(std::unordered_set<std::string>());
  }
  NextStore next_store = NextStore(std::make_shared<std::vector<std::unordered_set<std::string>>>(stmt_vector), std::make_shared<std::unordered_map<std::string, StmtType>>());
  next_store.AddNext(LINE_1, LINE_8);
  next_store.AddNext(LINE_1, LINE_2);
  next_store.AddNext(LINE_2, LINE_7);
  next_store.AddNext(LINE_2, LINE_3);
  next_store.AddNext(LINE_3, LINE_5);
  next_store.AddNext(LINE_3, LINE_4);
  next_store.AddNext(LINE_5, LINE_6);
  next_store.AddNext(LINE_6, LINE_5);
  next_store.AddNext(LINE_8, LINE_9);
  next_store.AddNext(LINE_9, LINE_10);
  next_store.AddNext(LINE_9, LINE_11);
  next_store.AddNext(LINE_10, LINE_15);
  next_store.AddNext(LINE_11, LINE_12);
  next_store.AddNext(LINE_12, LINE_13);
  next_store.AddNext(LINE_12, LINE_14);
  next_store.AddNext(LINE_13, LINE_12);
  next_store.AddNext(LINE_14, LINE_15);
  next_store.AddNext(LINE_15, LINE_8);
  return next_store;
}

TEST_CASE("Valid next pair") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "9";
  std::string stmt2 = "11";
  bool actual = next_store.IsNextValid({stmt1, stmt2});
  bool expected = all_next_pairs.find({stmt1, stmt2}) != all_next_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid next pair") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "4";
  std::string stmt2 = "5";
  bool actual = next_store.IsNextValid({stmt1, stmt2});
  bool expected = all_next_pairs.find({stmt1, stmt2}) != all_next_pairs.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Valid next list") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "3";
  std::unordered_set<std::string> actual;
  actual.insert("4");
  actual.insert("5");
  std::unordered_set<std::string> expected = next_store.GetNextOf(stmt1);

  REQUIRE(actual == expected);
}

TEST_CASE("Valid before of stmt") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "15";
  std::unordered_set<std::string> actual = before_map.at(stmt1);
  std::unordered_set<std::string> expected = next_store.GetBeforeOf(stmt1);

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid before of stmt") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "15";
  std::unordered_set<std::string> actual;
  actual.insert("9");
  actual.insert("10");
  actual.insert("14");
  std::unordered_set<std::string> expected = next_store.GetBeforeOf(stmt1);

  REQUIRE(actual != expected);
}

