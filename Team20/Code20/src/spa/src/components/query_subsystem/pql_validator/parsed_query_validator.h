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
  bool IsProc(DesignEntityType);
  bool IsEntRef(PqlTokenType);
  bool IsExpressionSpec(PqlTokenType);
  bool IsNameAttribute(AtrriName attri_name);
  bool IsIntegerAttribute(AtrriName attri_name);
  bool ValidateAttribute(PqlToken, std::unordered_map<std::string, DesignEntityType>);

  bool ValidateResultClauseDeclared(ParsedQuery);

  bool ValidateSuchThatClause(ParsedQuery);
  bool ValidateFollowsFollowsTArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateParentParentTArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateCallsCallsTArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateNextNextTArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateAffectsAffectsTArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateUsesArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateModifiesArguments(Relationship, std::unordered_map<std::string, DesignEntityType>);

  bool ValidatePatternClause(ParsedQuery);
  bool ValidateAssignPatternArguments(Pattern, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateWhilePatternArguments(Pattern, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateIfPatternArguments(Pattern, std::unordered_map<std::string, DesignEntityType>);

  bool ValidateWithClause(ParsedQuery);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
