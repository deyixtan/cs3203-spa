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

TEST_CASE("Test merge empty table with non-empty table") {
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
  pql::Table empty_table;
  pql::Table single_attribute_table{test_synonym, test_single_constraints};
  empty_table.Merge(single_attribute_table);
//  std::cout << empty_table;
  REQUIRE(empty_table.records.empty()==true);
}

TEST_CASE("Test merge non-empty table with empty table") {
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
  pql::Table empty_table;
  pql::Table single_attribute_table{test_synonym, test_single_constraints};
  single_attribute_table.Merge(empty_table);
//  std::cout << single_attribute_table;
  REQUIRE(single_attribute_table.records.empty()==true);
}

TEST_CASE("Test merge two non-empty tables with one common attribute") {
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

  std::string test_synonym_other = "v";
  std::unordered_set<std::pair<std::string,std::string>, pair_hash> pair_constraints{
      {"1", "a"},
      {"12", "b"},
      {"3", "c"},
      {"14", "d"},
      {"5", "e"},
      {"16", "f"},
      {"7", "g"},
      {"18", "h"},
      {"9", "i"}
  };
  pql::Table two_attribute_table{test_synonym, test_synonym_other, pair_constraints};
  single_attribute_table.Merge(two_attribute_table);
//  std::cout << single_attribute_table;
  REQUIRE(single_attribute_table.attributes.size()==2);
  REQUIRE(single_attribute_table.records.size()==5);
}

TEST_CASE("Test merge two tables with two common attributes") {
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
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> test_pair_constraints2{
      {"2", "b"},
      {"4", "d"},
      {"6", "f"},
      {"8", "h"},
  };
  pql::Table two_attribute_table{test_first_synonym, test_second_synonym, test_pair_constraints};
  pql::Table two_attribute_table2{test_first_synonym, test_second_synonym, test_pair_constraints2};
  two_attribute_table.Merge(two_attribute_table2);
//  std::cout << two_attribute_table;
  REQUIRE(two_attribute_table.attributes.size()==2);
  REQUIRE(two_attribute_table.records.size()==4);
}