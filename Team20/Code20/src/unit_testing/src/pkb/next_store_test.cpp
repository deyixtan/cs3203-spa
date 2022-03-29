#include "components/pkb/stores/next_store.h"
#include "next_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"

NextStore set_up_next() {
  std::vector<std::unordered_set<std::string>> stmt_vector;
  for (int i = 0; i < COUNT; i++) {
    stmt_vector.push_back(std::unordered_set<std::string>());
  }
  NextStore next_store = NextStore(std::make_shared<std::vector<std::unordered_set<std::string>>>(stmt_vector));
  next_store.AddNextMap(next_map);
  next_store.AddBeforeMap(before_map);
  next_store.ConstructNextPairs();

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

TEST_CASE("Valid next stmt") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "8";
  bool actual = next_store.IsNext(stmt1);

  REQUIRE(actual == true);
}

TEST_CASE("Invalid next stmt") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "16";
  bool actual = next_store.IsNext(stmt1);

  REQUIRE(actual == false);
}

TEST_CASE("Valid before stmt") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "9";
  bool actual = next_store.IsBefore(stmt1);

  REQUIRE(actual == true);
}

TEST_CASE("Invalid before stmt") {
  NextStore next_store = set_up_next();
  std::string stmt1 = "16";
  bool actual = next_store.IsBefore(stmt1);

  REQUIRE(actual == false);
}