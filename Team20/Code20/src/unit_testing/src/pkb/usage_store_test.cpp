#include "usage_store_test.h"
#include "components/pkb/usage_store.h"

#include "catch.hpp"

void setUp() {
  UsageStore usage_store;
  usage_store.add_stmt_var(1, "red");
  usage_store.add_stmt_var(2, "green");
  usage_store.add_stmt_var(3, "blue");
}

TEST_CASE("Add statement variable") {

}