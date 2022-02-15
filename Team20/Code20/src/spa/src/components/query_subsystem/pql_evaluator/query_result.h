#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "query_condition.h"
#include "components/pkb/hash.h"

class QueryResult {
 public:
  QueryResult();
  QueryResult(std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>>);
  std::unordered_set<std::string> GetResult(PqlToken selected_synonym);

 private:
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetIntersection(
      std::unordered_set<std::pair<std::string, std::string>, pair_hash> first_set,
      std::unordered_set<std::pair<std::string, std::string>, pair_hash> second_set
      );
  std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> result_table;
};

#endif // QUERY_RESULT_H
