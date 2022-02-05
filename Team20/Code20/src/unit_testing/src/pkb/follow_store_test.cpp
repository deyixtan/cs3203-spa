#include "components/pkb/follow_store.h"
#include "follow_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"

FollowStore follow_store;

void set_up_follow(int numStmts) {
  follow_store.rs_init(numStmts);
  follow_store.add_follow(LINE_1, LINE_2);
  follow_store.add_follow(LINE_2, LINE_3);
  follow_store.add_follow(LINE_3, LINE_4);
  follow_store.add_follow(LINE_4, LINE_7);
  follow_store.add_follow(LINE_5, LINE_6);
  follow_store.add_follow(LINE_6, LINE_7);
  follow_store.add_follow(LINE_7, LINE_8);
  follow_store.add_follow(LINE_8, LINE_10);
  follow_store.add_follow(LINE_9, LINE_10);
}

TEST_CASE("Valid follow pair") {
  set_up_follow(10);
  int stmt1 = 2;
  int stmt2 = 3;
  bool actual = follow_store.follow_exists(stmt1, stmt2);
  bool expected = follow_set.at(stmt1) == stmt2;

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid follow pair") {
  set_up_follow(10);
  int stmt1 = 2;
  int stmt2 = 5;
  bool actual = follow_store.follow_exists(stmt1, stmt2);
  bool expected = follow_set.at(stmt1) == stmt2;

  REQUIRE(actual == expected);
}

TEST_CASE("Get follower of a statement (Correct)") {
  set_up_follow(10);
  int num = 2;
  int actual = follow_store.get_follower_of(num);
  int expected = rs_map.at(num).follower;

  REQUIRE(actual == expected);
}

TEST_CASE("Get follower of a statement (Wrong)") {
  set_up_follow(10);
  int num = 2;
  int actual = follow_store.get_follower_of(num);
  int expected = rs_map.at(8).follower;

  REQUIRE(actual != expected);
}

TEST_CASE("Get following of a statement (Correct)") {
  set_up_follow(10);
  int num = 5;
  int actual = follow_store.get_following_of(num);
  int expected = rs_map.at(num).following;

  REQUIRE(actual == expected);
}

TEST_CASE("Get following of a statement (Wrong)") {
  set_up_follow(10);
  int num = 5;
  int actual = follow_store.get_following_of(num);
  int expected = rs_map.at(8).following;

  REQUIRE(actual != expected);
}