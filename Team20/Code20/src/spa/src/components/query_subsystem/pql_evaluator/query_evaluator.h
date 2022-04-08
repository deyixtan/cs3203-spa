#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../../pkb/pkb.h"
#include "query_condition.h"
#include "clause.h"
#include "clause_factory.h"
#include "clause_util.h"

#include <string>
#include <unordered_set>
#include <memory>
#include <list>
#include <queue>

namespace pql {

class QueryEvaluator {
 public:
  static void Evaluate(ParsedQuery &query, const PkbPtr &pkb, std::list<std::string> &results);

 private:
  static std::queue<std::shared_ptr<pql::Clause> > ExtractClauses(ParsedQuery &query, const PkbPtr &pkb);
};

}

#endif //QUERY_EVALUATOR_H_
