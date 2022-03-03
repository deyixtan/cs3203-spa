#include "sample_token.h"
#include "components/query_subsystem/pql_evaluator/query_condition.h"
#include "components/query_subsystem/pql_evaluator/query_result.h"

#include <vector>
#include <utility>
#include <unordered_set>
#include "catch.hpp"

std::pair<std::string, std::string> pair_1x = std::make_pair("1", "x");
std::pair<std::string, std::string> pair_2x = std::make_pair("2", "x");
std::pair<std::string, std::string> pair_3x = std::make_pair("3", "x");
std::pair<std::string, std::string> pair_4x = std::make_pair("4", "x");
std::pair<std::string, std::string> pair_5x = std::make_pair("5", "x");
std::pair<std::string, std::string> pair_1y = std::make_pair("1", "y");
std::pair<std::string, std::string> pair_2y = std::make_pair("2", "y");
std::pair<std::string, std::string> pair_3y = std::make_pair("3", "y");
std::pair<std::string, std::string> pair_4y = std::make_pair("4", "y");
std::pair<std::string, std::string> pair_5y = std::make_pair("5", "y");
std::pair<std::string, std::string> pair_12 = std::make_pair("1", "2");
std::pair<std::string, std::string> pair_13 = std::make_pair("1", "3");
std::pair<std::string, std::string> pair_14 = std::make_pair("1", "4");
std::pair<std::string, std::string> pair_1_string_x = std::make_pair("1", "\"x\"");
std::pair<std::string, std::string> pair_2_string_x = std::make_pair("2", "\"x\"");
std::pair<std::string, std::string> pair_3_string_x = std::make_pair("3", "\"x\"");
std::pair<std::string, std::string> pair_4_string_x = std::make_pair("4", "\"x\"");
std::pair<std::string, std::string> pair_5_string_x = std::make_pair("5", "\"x\"");

TEST_CASE("Test 2 synonyms 2 common synonyms") {
  /*
   *   (a, v)  |  (a, v)  |  Select a  |
   *  ------------------------------------
   *   (1, x)  |  (1, x)  |  1
   *   (2, x)  |  (2, x)  |  2
   *   (3, x)  |  (3, y)  |
   */

  QueryCondition condition = QueryCondition(a_token, v_token);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> first_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> second_set;
  first_set.insert(pair_1x);
  first_set.insert(pair_2x);
  first_set.insert(pair_3x);
  second_set.insert(pair_1x);
  second_set.insert(pair_2x);
  second_set.insert(pair_3y);

  std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> result_table;
  result_table.push_back(std::make_pair(condition, first_set));
  result_table.push_back(std::make_pair(condition, second_set));

  std::unordered_set<std::string> expected_result = {
      "1",
      "2"
  };

  QueryResult query_result = QueryResult(result_table);
  std::unordered_set<std::string> test_result = query_result.GetResult(a_token);

  REQUIRE(test_result == expected_result);
}

TEST_CASE("Test 2 synonym 1 common synonym") {
  /*
   *   Select a such that Follows* (1, a) pattern a ("x", _)
   *
   *   (1, a)  |  (a, "x")  |  Select a  |
   *  ------------------------------------
   *   (1, 2)  |  (1, "x")  |  2
   *   (1, 3)  |  (2, "x")  |  4
   *   (1, 4)  |  (4, "x")  |
   */

  QueryCondition first_condition = QueryCondition(number_value_token_1, a_token);
  QueryCondition second_condition = QueryCondition(a_token, x_string_token);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> first_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> second_set;
  first_set.insert(pair_12);
  first_set.insert(pair_13);
  first_set.insert(pair_14);
  second_set.insert(pair_1_string_x);
  second_set.insert(pair_2_string_x);
  second_set.insert(pair_4_string_x);

  std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> result_table;
  result_table.push_back(std::make_pair(first_condition, first_set));
  result_table.push_back(std::make_pair(second_condition, second_set));

  std::unordered_set<std::string> expected_result = {
      "2",
      "4"
  };

  QueryResult query_result = QueryResult(result_table);
  std::unordered_set<std::string> test_result = query_result.GetResult(a_token);

  REQUIRE(test_result == expected_result);
}

TEST_CASE("Test 3 synonyms 1 common synonym") {
  /*
   *   (a1, v)  |  (a, v)  |  Select a  |
   *  ------------------------------------
   *   (1, x)  |  (1, x)  |  1
   *   (2, x)  |  (2, x)  |  2
   *   (3, x)  |  (3, y)  |
   */

  QueryCondition first_condition = QueryCondition(a1_token, v_token);
  QueryCondition second_condition = QueryCondition(a_token, v_token);
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> first_set;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> second_set;
  first_set.insert(pair_1x);
  first_set.insert(pair_2x);
  first_set.insert(pair_3x);
  second_set.insert(pair_1x);
  second_set.insert(pair_2x);
  second_set.insert(pair_3y);

  std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> result_table;
  result_table.push_back(std::make_pair(first_condition, first_set));
  result_table.push_back(std::make_pair(second_condition, second_set));

  std::unordered_set<std::string> expected_result = {
      "1",
      "2"
  };

  QueryResult query_result = QueryResult(result_table);
  std::unordered_set<std::string> test_result = query_result.GetResult(a_token);

  REQUIRE(test_result == expected_result);
}
