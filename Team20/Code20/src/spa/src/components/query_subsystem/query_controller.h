#ifndef QUERY_CONTROLLER_H
#define QUERY_CONTROLLER_H

#include "./pql_validator/parsed_query_validator.h"
#include "./pql_evaluator/query_evaluator.h"
#include "../pkb/pkb.h"

#include <list>
#include <string>
#include <memory>
#include <iostream>

namespace pql {

using pql_validator::ParsedQueryValidator;
using pql_evaluator::QueryEvaluator;

class QueryController {
 public:
  static void ProcessQuery(std::string query, const PkbPtr &pkb, std::list<std::string> &results);
};

}

#endif
