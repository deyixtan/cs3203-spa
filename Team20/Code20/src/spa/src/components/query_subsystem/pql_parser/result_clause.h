#ifndef RESULT_CLAUSE_H
#define RESULT_CLAUSE_H
#include "../pql_lexer/pql_token.h"

enum class ResultClauseType {
  BOOLEAN,
  TUPLE,
  SYNONYM
};

class ResultClause {
 private:
  ResultClauseType type;
  std::vector<PqlToken> values;
 public:
  ResultClause();
  ResultClause(ResultClauseType, std::vector<PqlToken>);
  ResultClauseType GetType();
  std::vector<PqlToken> GetValues();
};

#endif //RESULT_CLAUSE_H
