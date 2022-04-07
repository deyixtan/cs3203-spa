#ifndef PATTERN_H
#define PATTERN_H

#include "../pql_lexer/pql_token.h"

using namespace pql;

class Pattern {
 private:
  PqlToken synonym;
  PqlToken first;
  PqlToken second;
  PqlToken third;

 public:
  Pattern();
  Pattern(PqlToken, PqlToken, PqlToken);
  Pattern(PqlToken, PqlToken, PqlToken, PqlToken); // For ifs only
  PqlToken GetSynonym();
  PqlToken GetFirst();
  PqlToken GetSecond();
  PqlToken GetThird();
};

#endif
