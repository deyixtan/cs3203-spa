#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../../pkb/pkb.h"
#include "query_condition.h"
#include "query_result.h"

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
  void EvaluateSelectOnly(ParsedQuery&  query);
  void EvaluateSelectWithRelationship(ParsedQuery& query);
  void EvaluateSelectWithPattern(ParsedQuery& query);
  void EvaluateSelectWithRelationshipAndPattern(ParsedQuery& query);
};

}

#endif //QUERY_EVALUATOR_H_
