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
  bool IsStmt(DesignEntityType);
  bool IsStmtRef(PqlTokenType);
  bool IsEntRef(PqlTokenType);
  bool IsExpressionSpec(PqlTokenType);
  bool ValidateResultClauseDeclared(ParsedQuery);
  bool ValidateSuchThatClause(ParsedQuery);
  bool ValidateFollowsFollowsTArguments(ParsedQuery);
  bool ValidateParentParentTArguments(ParsedQuery);
  bool ValidateUsesArguments(ParsedQuery);
  bool ValidateModifiesArguments(ParsedQuery);
  bool ValidatePatternClause(ParsedQuery);
  bool ValidatePatternSynonymIsAssigned(ParsedQuery);
  bool ValidatePatternArguments(ParsedQuery);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
