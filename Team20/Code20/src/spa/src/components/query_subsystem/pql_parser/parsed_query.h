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
  ParsedQuery(PqlToken, std::vector<Declaration>, Relationship, Pattern);
};

#endif
