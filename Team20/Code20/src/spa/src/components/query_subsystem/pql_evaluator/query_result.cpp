#include "query_result.h"

QueryResult::QueryResult() {};
QueryResult::QueryResult(
    std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>>>> result
    ) : result(result) {};

std::unordered_set<std::string> QueryResult::GetResult(PqlToken selected_synonym) {
  std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>>> first_condition_pair = result[0];
  std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>>> second_condition_pair = result[1];

  std::unordered_set<std::string> result;

  std::unordered_set<std::pair<std::string, std::string>> intersection;
  std::vector<std::string> result_list;

  if (first_condition_pair.first == second_condition_pair.first) {
    intersection = GetIntersection(first_condition_pair.second, second_condition_pair.second);
    if (first_condition_pair.first.first == selected_synonym) {
     for (auto result_pair : intersection) {
       result.insert(result_pair.first);
     }
    } else {
      for (auto result_pair : intersection) {
        result.insert(result_pair.second);
      }
    }
  } else {
    if (first_condition_pair.first.first == selected_synonym) {
      for (auto result_pair : first_condition_pair.second) {
        result_list.push_back(result_pair.first);
      }
    }
    if (first_condition_pair.first.second == selected_synonym) {
      for (auto result_pair : first_condition_pair.second) {
        result_list.push_back(result_pair.second);
      }
    }
    if (second_condition_pair.first.first == selected_synonym) {
      for (auto result_pair : second_condition_pair.second) {
        result_list.push_back(result_pair.first);
      }
    }
    if (second_condition_pair.first.second == selected_synonym) {
      for (auto result_pair : second_condition_pair.second) {
        result_list.push_back(result_pair.second);
      }
    }

    std::unordered_map<std::string, int> result_map;

    for (auto result_string : result_list) {
      if (result_map.find(result_string) == result_map.end()) {
        result_map.insert(make_pair(result_string, 1));
      } else {
        result_map.insert(std::make_pair(result_string, result_map[result_string] + 1));
      }
    }

    for (auto key_value_pair : result_map) {
      if (key_value_pair.second > 1) {
        result.insert(key_value_pair.first);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>> QueryResult::GetIntersection(
    std::unordered_set<std::pair<std::string, std::string>> first_set,
    std::unordered_set<std::pair<std::string, std::string>> second_set
    ) {

  std::unordered_set<std::pair<std::string, std::string>> intersection;
  for (auto result_pair : first_set) {
    if (second_set.find(result_pair) != second_set.end()) {
      intersection.insert(result_pair);
    }
  }
  return intersection;
}