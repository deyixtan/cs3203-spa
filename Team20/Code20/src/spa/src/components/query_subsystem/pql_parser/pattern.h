#ifndef PATTERN_H
#define PATTERN_H

#include "../pql_lexer/pql_token.h"

class Pattern {
 private:
  PqlToken syn_assign_;
  PqlToken first_;
  PqlToken second_;
  PqlToken third_;

 public:
  Pattern();
  Pattern(PqlToken, PqlToken, PqlToken);
  Pattern(PqlToken, PqlToken, PqlToken, PqlToken); // For ifs only
  PqlToken GetSynAssign();
  PqlToken GetFirst();
  PqlToken GetSecond();
  PqlToken GetThird();
};

#endif
