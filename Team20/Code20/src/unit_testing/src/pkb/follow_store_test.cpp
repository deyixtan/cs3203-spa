#include "components/pkb/follow_store.h"
#include "follow_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"

FollowStore follow_store;

void set_up_follow(int numStmts) {
  follow_store.Init(numStmts);
  follow_store.AddFollow(LINE_1, LINE_2);
  follow_store.AddFollow(LINE_2, LINE_3);
  follow_store.AddFollow(LINE_3, LINE_4);
  follow_store.AddFollow(LINE_4, LINE_7);
  follow_store.AddFollow(LINE_5, LINE_6);
  follow_store.AddFollow(LINE_6, LINE_7);
  follow_store.AddFollow(LINE_7, LINE_8);
  follow_store.AddFollow(LINE_8, LINE_10);
  follow_store.AddFollow(LINE_9, LINE_10);
}

TEST_CASE("Valid follow pair") {
  set_up_follow(10);
  std::string stmt1 = "2";
  std::string stmt2 = "3";
  bool actual = follow_store.FollowExists({stmt1, stmt2});
  bool expected = follow_set.find({stmt1, stmt2}) != follow_set.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid follow pair") {
  set_up_follow(10);
  std::string stmt1 = "2";
  std::string stmt2 = "5";
  bool actual = follow_store.FollowExists({stmt1, stmt2});
  bool expected = follow_set.find({stmt1, stmt2}) != follow_set.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Get follower of a statement (Correct)") {
  set_up_follow(10);
  std::string num = "2";
  std::string actual = follow_store.GetFollowerOf(num);
  std::string expected = rs_map.at(num).follower;

  REQUIRE(actual == expected);
}

TEST_CASE("Get follower of a statement (Wrong)") {
  set_up_follow(10);
  std::string num = "2";
  std::string actual = follow_store.GetFollowerOf(num);
  std::string expected = rs_map.at("8").follower;

  REQUIRE(actual != expected);
}

TEST_CASE("Get following of a statement (Correct)") {
  set_up_follow(10);
  std::string num = "5";
  std::string actual = follow_store.GetFollowingOf(num);
  std::string expected = rs_map.at(num).following;

  REQUIRE(actual == expected);
}

TEST_CASE("Get following of a statement (Wrong)") {
  set_up_follow(10);
  std::string num = "5";
  std::string actual = follow_store.GetFollowingOf(num);
  std::string expected = rs_map.at("8").following;

  REQUIRE(actual != expected);
}

TEST_CASE("Get follower* of a statement (Correct)") {
  //to be implemented
}

TEST_CASE("Get follower* of a statement (Wrong)") {
  //to be implemented
}

TEST_CASE("Get following* of a statement (Correct)") {
  //to be implemented
}

TEST_CASE("Get following* of a statement (Wrong)") {
  //to be implemented
}