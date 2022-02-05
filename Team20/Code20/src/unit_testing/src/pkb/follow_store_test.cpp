#include "components/pkb/follow_store.h"
#include "follow_store_stub.h"
#include "../test_util.h"

#include "catch.hpp"

FollowStore follow_store;

void set_up() {
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

TEST_CASE("Get follower of a statement (Correct)") {
  set_up();
  int num = 5;
  int actual = follow_store.get_following_of(num);
  int expected = rs_map.at(5).follower;

  REQUIRE(actual == expected);
}

TEST_CASE("Get following of a statement (Correct)") {
  set_up();
  int num = 5;
  int actual = follow_store.get_following_of(num);
  int expected = rs_map.at(5).following;

  REQUIRE(actual == expected);
}