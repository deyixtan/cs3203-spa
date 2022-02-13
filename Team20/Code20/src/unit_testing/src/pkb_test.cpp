#include "pkb_stub.h"
#include "components/pkb/pkb.h"
#include "catch.hpp"

void set_up_pkb() {
  PKB *pkb = PKB::get_instance();
  pkb->add_stmt("a", VARS);
  pkb->add_stmt("b", VARS);
  pkb->add_stmt("c", VARS);
  pkb->add_stmt("d", VARS);
  pkb->add_stmt("e", VARS);
  pkb->add_stmt("f", VARS);
  pkb->add_stmt("g", VARS);
  pkb->add_stmt("h", VARS);
  pkb->add_stmt("i", VARS);
  pkb->add_stmt("j", VARS);

  pkb->add_stmt("1", STMT);
  pkb->add_stmt("2", STMT);
  pkb->add_stmt("3", STMT);
  pkb->add_stmt("4", STMT);
  pkb->add_stmt("5", STMT);
  pkb->add_stmt("6", STMT);
  pkb->add_stmt("7", STMT);
  pkb->add_stmt("8", STMT);
  pkb->add_stmt("9", STMT);
  pkb->add_stmt("10", STMT);

  pkb->add_stmt("1", ASSIGN);
  pkb->add_stmt("2", ASSIGN);
  pkb->add_stmt("3", ASSIGN);
  pkb->add_stmt("4", ASSIGN);
  pkb->add_stmt("5", ASSIGN);
  pkb->add_stmt("6", ASSIGN);
  pkb->add_stmt("7", ASSIGN);
  pkb->add_stmt("8", ASSIGN);
  pkb->add_stmt("9", ASSIGN);
  pkb->add_stmt("10", ASSIGN);
}

TEST_CASE("PKB instance") {
  PKB *pkb1 = PKB::get_instance();
  PKB *pkb2 = PKB::get_instance();

  REQUIRE(pkb1 == pkb2);
}

TEST_CASE("stmt s; select s;") {
  set_up_pkb();
  PKB *pkb = PKB::get_instance();
  auto actual = pkb->get_stmt(STMT);
  auto expected = stmt_list;

  REQUIRE(actual == expected);
}

TEST_CASE("variable v; select v;") {
  set_up_pkb();
  PKB *pkb = PKB::get_instance();
  auto actual = pkb->get_stmt(VARS);
  auto expected = var_list;

  REQUIRE(actual == expected);
}

TEST_CASE("assign a; select a;") {
  set_up_pkb();
  PKB *pkb = PKB::get_instance();
  auto actual = pkb->get_stmt(ASSIGN);
  auto expected = assign_stmt_list;

  REQUIRE(actual == expected);
}
