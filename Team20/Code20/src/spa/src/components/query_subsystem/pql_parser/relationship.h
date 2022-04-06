#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "../pql_lexer/pql_token.h"

class Relationship {
 private:
  PqlToken rel_ref;
  PqlToken first;
  PqlToken second;

 public:
  Relationship();
  Relationship(PqlToken rel_ref);
  Relationship(PqlToken, PqlToken, PqlToken);
  PqlToken GetRelRef();
  PqlToken GetFirst();
  PqlToken GetSecond();
  void SetFirst(PqlToken);
  void SetSecond(PqlToken);
};

#endif
