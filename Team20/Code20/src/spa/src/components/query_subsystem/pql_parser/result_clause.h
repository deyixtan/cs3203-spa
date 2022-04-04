#ifndef RESULT_CLAUSE_H
#define RESULT_CLAUSE_H
#include "../pql_lexer/pql_token.h"

enum class ResultClauseType {
  BOOLEAN,
  TUPLE,
  SYNONYM,
  ATTRIBUTE
};

class ResultClause {
 private:
  ResultClauseType type;
  std::vector<PqlToken> values;
 public:
  ResultClause();
  ResultClause(ResultClauseType);
  ResultClause(ResultClauseType, std::vector<PqlToken>);
  ResultClauseType GetType();
  std::vector<PqlToken> GetValues();
  void AddValue(PqlToken);
  void ModifyType(ResultClauseType);
};

#endif //RESULT_CLAUSE_H
