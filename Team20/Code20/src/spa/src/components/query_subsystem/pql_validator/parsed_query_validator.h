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
  bool ArePatternArgsSemanticallyValid(Pattern, std::unordered_map<std::string, DesignEntityType>);
  bool IsPatternAssignSemanticallyValid(Pattern);
  bool IsPatternWhileSemanticallyValid(Pattern);
  bool IsPatternIfSemanticallyValid(Pattern);

  bool IsWithClauseSemanticallyValid(ParsedQuery);
  bool IsAttributeAttributeSemanticallyValid(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
  bool IsAttributeOthersSemanticallyValid(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
  bool IsOthersAttributeSemanticallyValid(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
