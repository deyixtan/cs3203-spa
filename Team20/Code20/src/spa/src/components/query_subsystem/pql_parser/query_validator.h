#ifndef QUERY_VALIDATOR_H
#define QUERY_VALIDATOR_H

#include "../pql_lexer/pql_token.h"

class QueryValidator {
 private:
  std::vector<PqlToken> tokens_;
 public:
  QueryValidator(std::vector<PqlToken>);
  std::vector<PqlToken> CheckValidation();
};

#endif
