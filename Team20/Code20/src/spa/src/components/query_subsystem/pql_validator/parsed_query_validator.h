#ifndef PARSED_QUERY_VALIDATOR_H_
#define PARSED_QUERY_VALIDATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../pql_lexer/pql_token.h"
#include "../utils.h"

namespace pql_validator {

class ParsedQueryValidator {
 public:
  static bool IsQuerySemanticallyValid(ParsedQuery query);

 private:
  static bool IsNameAttribute(AttriName attri_name);
  static bool IsIntegerAttribute(AttriName attri_name);

  static bool IsDeclarationSemanticallyValid(Declaration);
  static bool IsResultClauseSemanticallyValid(ParsedQuery);
  static bool IsSynonymDeclared(PqlToken, std::unordered_map<std::string, DesignEntityType>);
  static bool IsAttributeSemanticallyValid(PqlToken token, std::unordered_map<std::string, DesignEntityType> declarations);
  static bool IsAttributeDeclared(std::pair<std::string, AttriName>, std::unordered_map<std::string, DesignEntityType>);

  static bool IsRelationshipClauseSemanticallyValid(ParsedQuery);
  static bool AreRelationshipArgsDeclared(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
  static bool AreRelationshipArgsSemanticallyValid(Relationship, std::unordered_map<std::string, DesignEntityType>);

  static bool IsPatternClauseSemanticallyValid(ParsedQuery);
  static bool ArePatternArgsSemanticallyValid(Pattern, std::unordered_map<std::string, DesignEntityType>);
  static bool IsPatternAssignSemanticallyValid(Pattern);
  static bool IsPatternWhileSemanticallyValid(Pattern);
  static bool IsPatternIfSemanticallyValid(Pattern);

  static bool IsWithClauseSemanticallyValid(ParsedQuery);
  static bool IsAttributeAttributeSemanticallyValid(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
  static bool IsAttributeOthersSemanticallyValid(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
  static bool IsOthersAttributeSemanticallyValid(PqlToken, PqlToken, std::unordered_map<std::string, DesignEntityType>);
};

};

#endif //PARSED_QUERY_VALIDATOR_H_
