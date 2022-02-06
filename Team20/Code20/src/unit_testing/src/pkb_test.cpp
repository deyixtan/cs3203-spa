#include "pkb_stub.h"
#include "components/pkb/pkb.h"
#include "catch.hpp"

void set_up_pkb() {
  PKB *pkb = PKB::get_instance();
  pkb->add_stmt(0, "a", VARS);
  pkb->add_stmt(0, "b", VARS);
  pkb->add_stmt(0, "c", VARS);
  pkb->add_stmt(0, "d", VARS);
  pkb->add_stmt(0, "e", VARS);
  pkb->add_stmt(0, "f", VARS);
  pkb->add_stmt(0, "g", VARS);
  pkb->add_stmt(0, "h", VARS);
  pkb->add_stmt(0, "i", VARS);
  pkb->add_stmt(0, "j", VARS);

  pkb->add_stmt(1, "", STMT);
  pkb->add_stmt(2, "", STMT);
  pkb->add_stmt(3, "", STMT);
  pkb->add_stmt(4, "", STMT);
  pkb->add_stmt(5, "", STMT);
  pkb->add_stmt(6, "", STMT);
  pkb->add_stmt(7, "", STMT);
  pkb->add_stmt(8, "", STMT);
  pkb->add_stmt(9, "", STMT);
  pkb->add_stmt(10, "", STMT);
}

//to be removed
TEST_CASE("PKB instance") {
  PKB *pkb1 = PKB::get_instance();
  PKB *pkb2 = PKB::get_instance();

  REQUIRE(pkb1 == pkb2);
}

//to be removed
TEST_CASE("stmt s; select s;") {
  set_up_pkb();
  PKB *pkb = PKB::get_instance();
  auto actual = pkb->temp_get_stmt();
  auto expected = stmt_list;

  REQUIRE(actual == expected);
}

//to be removed
TEST_CASE("variable v; select v;") {
  set_up_pkb();
  PKB *pkb = PKB::get_instance();
  auto actual = pkb->temp_get_vars();
  auto expected = var_list;

  REQUIRE(actual == expected);
}
