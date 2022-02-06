#ifndef DECLARATION_H
#define DECLARATION_H

#include "../pql_lexer/pql_token.h"

class Declaration {
 private:
  PqlToken synonym;
  PqlToken design_entity;
 public:
  Declaration();
  Declaration(PqlToken, PqlToken);
  PqlToken GetSynonym();
  PqlToken GetDesignEntity();
};

#endif
