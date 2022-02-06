#include "pattern.h"

Pattern::Pattern() {}
Pattern::Pattern(PqlToken syn_assign, PqlToken first, PqlToken second)
    : syn_assign_(syn_assign),
      first_(first),
      second_(second) {}

PqlToken Pattern::GetSynAssign() {
  return syn_assign_;
}

PqlToken Pattern::GetFirst() {
  return first_;
}

PqlToken Pattern::GetSecond() {
  return second_;
}