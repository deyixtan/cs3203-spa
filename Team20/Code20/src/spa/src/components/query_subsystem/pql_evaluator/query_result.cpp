#include "query_result.h"

QueryResult::QueryResult() {}
QueryResult::QueryResult(
    std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> result_table
    ) : result_table(result_table) {}

std::unordered_set<std::string> QueryResult::GetResult(PqlToken selected_synonym) {
  std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> first_condition_pair = result_table[0];
  std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> second_condition_pair = result_table[1];

  std::unordered_set<std::string> result;

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection;
  std::vector<std::string> result_list;

  if (first_condition_pair.first == second_condition_pair.first) {
    intersection = GetIntersection(first_condition_pair.second, second_condition_pair.second);
    if (first_condition_pair.first.first_arg == selected_synonym) {
     for (auto result_pair : intersection) {
       result.insert(result_pair.first);
     }
    } else {
      for (auto result_pair : intersection) {
        result.insert(result_pair.second);
      }
    }
  } else {
    std::unordered_set<std::string> synonym_set;
    std::string duplicated_synonym;

    if (!synonym_set.count(first_condition_pair.first.first_arg.value)) {
      synonym_set.insert(first_condition_pair.first.first_arg.value);
    } else {
      duplicated_synonym = first_condition_pair.first.first_arg.value;
    }

    if (!synonym_set.count(first_condition_pair.first.second_arg.value)) {
      synonym_set.insert(first_condition_pair.first.second_arg.value);
    } else {
      duplicated_synonym = first_condition_pair.first.second_arg.value;
    }

    if (!synonym_set.count(second_condition_pair.first.first_arg.value)) {
      synonym_set.insert(second_condition_pair.first.first_arg.value);
    } else {
      duplicated_synonym = second_condition_pair.first.first_arg.value;
    }

    if (!synonym_set.count(second_condition_pair.first.second_arg.value)) {
      synonym_set.insert(second_condition_pair.first.second_arg.value);
    } else {
      duplicated_synonym = second_condition_pair.first.second_arg.value;
    }

    std::unordered_set<std::string> single_value_set;
    if (synonym_set.size() < 4) {
      single_value_set = GetIntersectionSet(first_condition_pair, second_condition_pair, duplicated_synonym);
      // TODO
    } else {
      if (first_condition_pair.first.first_arg == selected_synonym) {
        for (auto result_pair : first_condition_pair.second) {
          result_list.push_back(result_pair.first);
        }
      }
      if (first_condition_pair.first.second_arg == selected_synonym) {
        for (auto result_pair : first_condition_pair.second) {
          result_list.push_back(result_pair.second);
        }
      }
      if (second_condition_pair.first.first_arg == selected_synonym) {
        for (auto result_pair : second_condition_pair.second) {
          result_list.push_back(result_pair.first);
        }
      }
      if (second_condition_pair.first.second_arg == selected_synonym) {
        for (auto result_pair : second_condition_pair.second) {
          result_list.push_back(result_pair.second);
        }
      }
      for (auto key_value_pair : result_list) {
        result.insert(key_value_pair);
      }
    }


//    int synonym_count = 0;
//    if (first_condition_pair.first.first_arg == selected_synonym) {
//      for (auto result_pair : first_condition_pair.second) {
//        result_list.push_back(result_pair.first);
//      }
//      synonym_count++;
//    }
//    if (first_condition_pair.first.second_arg == selected_synonym) {
//      for (auto result_pair : first_condition_pair.second) {
//        result_list.push_back(result_pair.second);
//      }
//      synonym_count++;
//    }
//    if (second_condition_pair.first.first_arg == selected_synonym) {
//      for (auto result_pair : second_condition_pair.second) {
//        result_list.push_back(result_pair.first);
//      }
//      synonym_count++;
//    }
//    if (second_condition_pair.first.second_arg == selected_synonym) {
//      for (auto result_pair : second_condition_pair.second) {
//        result_list.push_back(result_pair.second);
//      }
//      synonym_count++;
//    }
//
//    if (synonym_count > 1) {
//      std::unordered_map<std::string, int> result_map;
//
//      for (auto result_string : result_list) {
//        if (result_map.find(result_string) == result_map.end()) {
//          result_map.insert(make_pair(result_string, 1));
//        } else {
//          result_map.find(result_string)->second = result_map.at(result_string) + 1;
//        }
//      }
//
//      for (auto key_value_pair : result_map) {
//        if (key_value_pair.second > 1) {
//          result.insert(key_value_pair.first);
//        }
//      }
//    } else {
//      for (auto key_value_pair : result_list) {
//        result.insert(key_value_pair);
//      }
//    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> QueryResult::GetIntersection(
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> first_set,
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> second_set
    ) {

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection;
  for (auto result_pair : first_set) {
    if (second_set.find(result_pair) != second_set.end()) {
      intersection.insert(result_pair);
    }
  }
  return intersection;
}

std::unordered_set<std::string> QueryResult::GetIntersectionSet(
    std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> first_condition_pair,
    std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> second_condition_pair,
    std::string synonym) {

  std::vector<std::string> result_list;

  int synonym_count = 0;
  if (first_condition_pair.first.first_arg.value == synonym) {
    for (auto result_pair : first_condition_pair.second) {
      result_list.push_back(result_pair.first);
    }
    synonym_count++;
  }
  if (first_condition_pair.first.second_arg.value == synonym) {
    for (auto result_pair : first_condition_pair.second) {
      result_list.push_back(result_pair.second);
    }
    synonym_count++;
  }
  if (second_condition_pair.first.first_arg.value == synonym) {
    for (auto result_pair : second_condition_pair.second) {
      result_list.push_back(result_pair.first);
    }
    synonym_count++;
  }
  if (second_condition_pair.first.second_arg.value == synonym) {
    for (auto result_pair : second_condition_pair.second) {
      result_list.push_back(result_pair.second);
    }
    synonym_count++;
  }

  std::unordered_set<std::string> result;

  if (synonym_count > 1) {
    std::unordered_map<std::string, int> result_map;
    for (auto result_string : result_list) {
      if (result_map.find(result_string) == result_map.end()) {
        result_map.insert(make_pair(result_string, 1));
      } else {
        result_map.find(result_string)->second = result_map.at(result_string) + 1;
      }
    }

    for (auto key_value_pair : result_map) {
      if (key_value_pair.second > 1) {
        result.insert(key_value_pair.first);
      }
    }

    return result;
  }
}