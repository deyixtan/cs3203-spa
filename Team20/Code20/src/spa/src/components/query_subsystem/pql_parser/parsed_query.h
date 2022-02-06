#ifndef PARSER_PARSED_QUERY_H
#define PARSER_PARSED_QUERY_H

#include "../pql_lexer/pql_token.h"
#include "relationship.h"
#include "declaration.h"
#include "pattern.h"


class ParsedQuery {
 private:
  PqlToken selected_synonym;
  std::vector<Declaration> declarations;
  Relationship relationship;
  Pattern pattern;

 public:
  ParsedQuery();
  void SetSynonym(PqlToken);
  void SetPattern(Pattern);
  void SetRelationship(Relationship);
  void SetDeclarations(std::vector<Declaration>);
  PqlToken GetSynonym();
  std::vector<Declaration> GetDeclaration();
  Relationship GetRelationship();
  Pattern GetPattern();
};

#endif
