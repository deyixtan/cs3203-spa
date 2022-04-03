#ifndef PARSED_QUERY_VALIDATOR_H_
#define PARSED_QUERY_VALIDATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../pql_lexer/pql_token.h"

namespace pql_validator {

class ParsedQueryValidator {
 public:
  ParsedQueryValidator() {};
  bool IsQuerySemanticallyValid(ParsedQuery query);

 private:
  bool IsStmt(DesignEntityType);
  bool IsStmtRef(PqlTokenType);
  bool IsProc(DesignEntityType);
  bool IsEntRef(PqlTokenType);
  bool IsExpressionSpec(PqlTokenType);
  bool IsNameAttribute(AttriName attri_name);
  bool IsIntegerAttribute(AttriName attri_name);

  bool IsDeclarationSemanticallyValid(Declaration);
  bool IsResultClauseSemanticallyValid(ParsedQuery);
  bool IsSynonymDeclared(PqlToken, std::unordered_map<std::string, DesignEntityType>);
  bool IsAttributeSemanticallyValid(PqlToken token, std::unordered_map<std::string, DesignEntityType> declarations);
  bool IsAttributeDeclared(std::pair<std::string, AttriName>, std::unordered_map<std::string, DesignEntityType>);

  bool IsRelationshipClauseSemanticallyValid(ParsedQuery);
  bool AreRelationshipArgsDeclared(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
  bool AreRelationshipArgsSemanticallyValid(Relationship, std::unordered_map<std::string, DesignEntityType>);

  bool IsPatternClauseSemanticallyValid(ParsedQuery);
  bool ArePatternArgsSemanticallyValid(Pattern);

  bool ValidatePatternClause(ParsedQuery);
  bool ValidateAssignPatternArguments(Pattern, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateWhilePatternArguments(Pattern, std::unordered_map<std::string, DesignEntityType>);
  bool ValidateIfPatternArguments(Pattern, std::unordered_map<std::string, DesignEntityType>);

  bool ValidateWithClause(ParsedQuery);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
