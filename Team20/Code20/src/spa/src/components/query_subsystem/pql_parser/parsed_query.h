#ifndef PARSER_PARSED_QUERY_H
#define PARSER_PARSED_QUERY_H

#include "../pql_lexer/pql_token.h"
#include "relationship.h"
#include "declaration.h"
#include "pattern.h"

#include <optional>

class ParsedQuery {
 private:
  PqlToken selected_synonym;
  std::vector<Declaration> declarations;
  std::vector<Relationship> relationships;
  std::vector<Pattern> patterns;

 public:
  ParsedQuery();
  void SetSynonym(PqlToken);
  void AddPattern(Pattern);
  void AddRelationship(Relationship);
  void SetDeclarations(std::vector<Declaration>);
  PqlToken GetSynonym();
  std::vector<Declaration> GetDeclaration();
  std::vector<Relationship> GetRelationships();
  std::vector<Pattern> GetPatterns();
};

#endif
