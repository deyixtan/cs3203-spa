#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../../pkb/pkb.h"

#include <string>
#include <unordered_set>
#include <memory>
#include <list>

namespace pql_evaluator {

class QueryEvaluator {
 public:
  QueryEvaluator(PKB* pkb): pkb{pkb} {};
  void Evaluate(ParsedQuery&, std::list<std::string>&);

 private:
  std::unordered_set<std::string> result;
  PKB *pkb;
  void EvaluateSelect(ParsedQuery&);
  std::unordered_set<std::string> Intersect(std::unordered_set<std::string>, std::unordered_set<std::string>);
};

}

#endif //QUERY_EVALUATOR_H_
