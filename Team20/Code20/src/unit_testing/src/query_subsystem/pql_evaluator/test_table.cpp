#include "catch.hpp"

#include "components/query_subsystem/pql_evaluator/table.h"

TEST_CASE("Test empty table constructor") {
  pql::Table empty_table;
  REQUIRE(empty_table.attributes.empty()==true);
  REQUIRE(empty_table.records.empty()==true);
}

TEST_CASE("Test table constructor with one synonym") {
  std::string test_synonym = "s";
  std::unordered_set<std::string> test_single_constraints{
      "1",
      "2",
      "3",
      "4",
      "5",
      "6",
      "7",
      "8",
      "9"
  };
  pql::Table single_attribute_table{test_synonym, test_single_constraints};
  // std::cout << single_attribute_table;
  REQUIRE(single_attribute_table.attributes.empty()==false);
  REQUIRE(single_attribute_table.records.empty()==false);
}

TEST_CASE("Test table constructor with two synonyms") {
  std::string test_first_synonym = "s";
  std::string test_second_synonym = "v";
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> test_pair_constraints{
      {"1", "a"},
      {"2", "b"},
      {"3", "c"},
      {"4", "d"},
      {"5", "e"},
      {"6", "f"},
      {"7", "g"},
      {"8", "h"},
      {"9", "i"}
  };
  pql::Table two_attribute_table{test_first_synonym, test_second_synonym, test_pair_constraints};
  // std::cout << two_attribute_table;
  REQUIRE(two_attribute_table.attributes.empty()==false);
  REQUIRE(two_attribute_table.records.empty()==false);
}