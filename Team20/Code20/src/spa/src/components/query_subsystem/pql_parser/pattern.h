#ifndef PATTERN_H
#define PATTERN_H

#include "../pql_lexer/pql_token.h"

class Pattern {
 private:
  PqlToken syn_assign_;
  PqlToken first_;
  PqlToken second_;

 public:
  Pattern();
  Pattern(PqlToken, PqlToken, PqlToken);
  PqlToken GetSynAssign();
  PqlToken GetFirst();
  PqlToken GetSecond();
};

#endif
