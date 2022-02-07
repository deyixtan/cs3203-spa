#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../../pkb/pkb.h"

#include <string>
#include <unordered_set>
#include <memory>

namespace pql_evaluator {

class QueryEvaluator {
 public:
  QueryEvaluator() {};
  std::unordered_set<std::string> Evaluate(ParsedQuery&);

 private:
  std::unordered_set<std::string> result;
  std::shared_ptr<PKB> pkb;
  void EvaluateSelect(ParsedQuery&);
  std::unordered_set<std::string> Intersect(std::unordered_set<std::string>, std::unordered_set<std::string>);
};

}

#endif //QUERY_EVALUATOR_H_
