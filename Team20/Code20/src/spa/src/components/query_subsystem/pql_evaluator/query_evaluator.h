#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../../pkb/pkb.h"
#include "query_condition.h"
#include "clause.h"
#include "clause_factory.h"

#include <string>
#include <unordered_set>
#include <memory>
#include <list>
#include <queue>

namespace pql_evaluator {

class QueryEvaluator {
 public:
  explicit QueryEvaluator(PKB* pkb): pkb{pkb} {};
  void Evaluate(ParsedQuery&, std::list<std::string>&);

 private:
  std::unordered_set<std::string> result;
  PKB *pkb;
  std::queue<std::unique_ptr<pql::Clause> > ExtractClauses(ParsedQuery& query);
};

}

#endif //QUERY_EVALUATOR_H_
