#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "../pql_lexer/pql_token.h"

class Relationship {
 private:
  PqlToken rel_ref_;
  PqlToken first_;
  PqlToken second_;

 public:
  Relationship(PqlToken, PqlToken, PqlToken);
  PqlToken GetRelRef();
  PqlToken GetFirst();
  PqlToken GetSecond();
};

#endif
