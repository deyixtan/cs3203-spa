#ifndef QUERY_CONDITION_H
#define QUERY_CONDITION_H

#include "../pql_lexer/pql_token.h"

class QueryCondition {
 public:
  QueryCondition();
  QueryCondition(PqlToken, PqlToken);
  bool operator==(const QueryCondition &other) const {
    return first_arg == other.first_arg && second_arg == other.second_arg;
  }
  PqlToken first_arg;
  PqlToken second_arg;
};

#endif //QUERY_CONDITION_H
