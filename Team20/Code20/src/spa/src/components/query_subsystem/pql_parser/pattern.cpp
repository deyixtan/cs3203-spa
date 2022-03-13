#include "pattern.h"

Pattern::Pattern() {}
Pattern::Pattern(PqlToken syn_assign, PqlToken first, PqlToken second)
    : syn_assign_(syn_assign),
      first_(first),
      second_(second) {}
Pattern::Pattern(PqlToken syn_assign, PqlToken first, PqlToken second, PqlToken third)
    : syn_assign_(syn_assign),
      first_(first),
      second_(second),
      third_(third){}

PqlToken Pattern::GetSynAssign() {
  return syn_assign_;
}

PqlToken Pattern::GetFirst() {
  return first_;
}

PqlToken Pattern::GetSecond() {
  return second_;
}

PqlToken Pattern::GetThird() {
  return third_;
}