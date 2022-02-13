#ifndef QUERY_CONTROLLER_H
#define QUERY_CONTROLLER_H

#include "./pql_validator/parsed_query_validator.h"
#include "./pql_evaluator/query_evaluator.h"
#include "../pkb/pkb.h"

#include <list>
#include <string>
#include <memory>

using pql_validator::ParsedQueryValidator;
using pql_evaluator::QueryEvaluator;

class QueryController {
 public:
  QueryController(PKB *pkb);
  void ProcessQuery(std::string query, std::list<std::string> &results);

 private:
  std::unique_ptr<ParsedQueryValidator> validator_;
  std::unique_ptr<QueryEvaluator> evaluator_;
};

#endif
