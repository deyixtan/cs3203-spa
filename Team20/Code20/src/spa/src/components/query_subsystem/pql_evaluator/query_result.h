#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

enum class AllResult {
  ALL
};

class QueryResult {
 public:
  QueryResult();

 private:
  std::unordered_map<std::tuple<std::string, std::string>, std::unordered_set<std::tuple<int, std::string>>> result;
};

#endif // QUERY_RESULT_H
