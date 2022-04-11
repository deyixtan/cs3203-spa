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

class QueryController {
 public:
  static void ProcessQuery(std::string query, const pkb::PkbPtr &pkb, std::list<std::string> &results);
};

}

#endif
