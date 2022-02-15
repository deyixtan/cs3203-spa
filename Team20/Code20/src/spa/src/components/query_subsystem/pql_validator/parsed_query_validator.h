#ifndef PARSED_QUERY_VALIDATOR_H_
#define PARSED_QUERY_VALIDATOR_H_

#include "../pql_parser/parsed_query.h"

namespace pql_validator {

class ParsedQueryValidator {
 public:
  ParsedQueryValidator() {};
  void ValidateQuery(ParsedQuery);

 private:
  void ValidateNoDuplicateSynonymDeclared(ParsedQuery);
  void ValidateSelectSynonymDeclared(ParsedQuery);
  void ValidatePatternSynonymIsAssigned(ParsedQuery);
  void ValidateModifiesUsesFirstArgumentNotUnderscore(ParsedQuery);
  void ValidateClauseDesignEntity(ParsedQuery);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
