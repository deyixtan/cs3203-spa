#ifndef PARSER_PARSED_QUERY_H
#define PARSER_PARSED_QUERY_H

#include "../pql_lexer/pql_token.h"
#include "relationship.h"
#include "pattern.h"

#include <optional>

enum class DesignEntityType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE
};

class ParsedQuery {
 private:
  PqlToken selected_synonym;
  std::unordered_map<std::string, DesignEntityType> declarations;
  std::vector<Relationship> relationships;
  std::vector<Pattern> patterns;

 public:
  ParsedQuery();
  void SetSynonym(PqlToken);
  void AddPattern(Pattern);
  void AddRelationship(Relationship);
  void SetDeclarations(std::unordered_map<std::string, DesignEntityType>);
  PqlToken GetSynonym();
  std::unordered_map<std::string, DesignEntityType> GetDeclaration();
  std::vector<Relationship> GetRelationships();
  std::vector<Pattern> GetPatterns();
};

#endif
