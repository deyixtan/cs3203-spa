#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"

#include <string>
#include <unordered_set>

namespace pql_evaluator {

class QueryEvaluator {
 public:
  std::unordered_set<std::string> Evaluate(ParsedQuery&);

 private:
  std::unordered_set<std::string> result;
  void EvaluateSelect(ParsedQuery&);
  std::unordered_set<std::string> Intersect(std::unordered_set<std::string>, std::unordered_set<std::string>);
};

}

#endif //QUERY_EVALUATOR_H_
