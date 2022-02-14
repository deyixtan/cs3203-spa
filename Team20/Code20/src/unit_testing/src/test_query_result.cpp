#include "sample_token.h"
#include "components/query_subsystem/pql_evaluator/query_condition.h"
#include "components/query_subsystem/pql_evaluator/query_result.h"

#include <string>
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

TEST_CASE("Test same condition pair") {
  /*
   *   (a, v)  |  (a, v)  |  Select a  |
   *  ------------------------------------
   *   (1, x)  |  (1, x)  |  1
   *   (2, x)  |  (2, x)  |  2
   *   (3, x)  |  (3, y)  |
   */

  QueryCondition condition = QueryCondition(a_token, v_token);
  std::unordered_set<std::pair<std::string, std::string>> first_set;
  std::unordered_set<std::pair<std::string, std::string>> second_set;
  first_set.insert(pair_1x);
  first_set.insert(pair_2x);
  first_set.insert(pair_3x);
  second_set.insert(pair_1x);
  second_set.insert(pair_2x);
  second_set.insert(pair_3y);

  std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>>>> result_table;
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
