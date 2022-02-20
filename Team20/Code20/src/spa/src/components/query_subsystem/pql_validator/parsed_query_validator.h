#ifndef PARSED_QUERY_VALIDATOR_H_
#define PARSED_QUERY_VALIDATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../pql_lexer/pql_token.h"

namespace pql_validator {

class ParsedQueryValidator {
 public:
  ParsedQueryValidator() {};
  bool ValidateQuery(ParsedQuery);

 private:
  bool IsStmtRef(PqlTokenType);
  bool IsEntRef(PqlTokenType);
  bool IsExpressionSpec(PqlTokenType);
  bool ValidateNoDuplicateSynonymDeclared(ParsedQuery);
  bool ValidateSelectSynonymDeclared(ParsedQuery);
  bool ValidateSuchThatClause(ParsedQuery);
  bool ValidateFollowsFollowsTArguments(ParsedQuery);
  bool ValidateParentParentTArguments(ParsedQuery);
  bool ValidateUsesArguments(ParsedQuery);
  bool ValidateModifiesArguments(ParsedQuery);
  bool ValidatePatternClause(ParsedQuery);
  bool ValidatePatternSynonymIsAssigned(ParsedQuery);
  bool ValidatePatternArguments(ParsedQuery);
  bool ValidatePatternFirstArgumentIsEntRef(ParsedQuery);
  bool ValidatePatternSecondArgumentIsExpressionSpec(ParsedQuery);
  bool ValidateModifiesUsesFirstArgumentNotUnderscore(ParsedQuery);
  bool ValidateClauseDesignEntity(ParsedQuery);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
