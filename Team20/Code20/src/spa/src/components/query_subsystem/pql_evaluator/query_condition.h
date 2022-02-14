#ifndef QUERY_CONDITION_H
#define QUERY_CONDITION_H

#include "../pql_lexer/pql_token.h"

class QueryCondition {
 public:
  QueryCondition();
  QueryCondition(PqlToken, PqlToken);
  bool operator==(const QueryCondition &other) const {
    return first == other.first && second == other.second;
  }
  PqlToken first;
  PqlToken second;
};

#endif //QUERY_CONDITION_H
