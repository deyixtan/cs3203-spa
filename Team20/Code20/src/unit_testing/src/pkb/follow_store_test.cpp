#include "components/pkb/stores/follows_store/follows_store.h"
#include "follow_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"
#include "components/pkb/pkb.h"

using namespace pkb;

std::shared_ptr<FollowsStore> set_up_follow() {
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

  pkb->GetFollowsStore()->AddFollow(LINE_1, LINE_2);
  pkb->GetFollowsStore()->AddFollow(LINE_2, LINE_3);
  pkb->GetFollowsStore()->AddFollow(LINE_3, LINE_4);
  pkb->GetFollowsStore()->AddFollow(LINE_4, LINE_7);
  pkb->GetFollowsStore()->AddFollow(LINE_5, LINE_6);
  pkb->GetFollowsStore()->AddFollow(LINE_6, LINE_7);
  pkb->GetFollowsStore()->AddFollow(LINE_7, LINE_8);
  pkb->GetFollowsStore()->AddFollow(LINE_8, LINE_10);
  pkb->GetFollowsStore()->AddFollow(LINE_9, LINE_10);
  return pkb->GetFollowsStore();
}

TEST_CASE("Valid follow pair") {
  std::shared_ptr<FollowsStore> follow_store = set_up_follow();
  std::string stmt1 = "2";
  std::string stmt2 = "3";
  bool actual = follow_store->IsFollowsPairValid({stmt1, stmt2});
  bool expected = follow_set.find({stmt1, stmt2}) != follow_set.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Invalid follow pair") {
  std::shared_ptr<FollowsStore> follow_store = set_up_follow();
  std::string stmt1 = "2";
  std::string stmt2 = "5";
  bool actual = follow_store->IsFollowsPairValid({stmt1, stmt2});
  bool expected = follow_set.find({stmt1, stmt2}) != follow_set.end();

  REQUIRE(actual == expected);
}

TEST_CASE("Get follower of a statement (Correct)") {
  std::shared_ptr<FollowsStore> follow_store = set_up_follow();
  std::string num = "2";
  std::string actual = follow_store->GetFollowerOf(STMT, num);
  std::string expected = rs_map.at(num).follower;

  REQUIRE(actual == expected);
}

TEST_CASE("Get follower of a statement (Wrong)") {
  std::shared_ptr<FollowsStore> follow_store = set_up_follow();
  std::string num = "2";
  std::string actual = follow_store->GetFollowerOf(STMT, num);
  std::string expected = rs_map.at("8").follower;

  REQUIRE(actual != expected);
}

TEST_CASE("Get following of a statement (Correct)") {
  std::shared_ptr<FollowsStore> follow_store = set_up_follow();
  std::string num = "5";
  std::string actual = follow_store->GetFollowingOf(STMT, num);
  std::string expected = rs_map.at(num).following;

  REQUIRE(actual == expected);
}

TEST_CASE("Get following of a statement (Wrong)") {
  std::shared_ptr<FollowsStore> follow_store = set_up_follow();
  std::string num = "5";
  std::string actual = follow_store->GetFollowingOf(STMT, num);
  std::string expected = rs_map.at("8").following;

  REQUIRE(actual != expected);
}