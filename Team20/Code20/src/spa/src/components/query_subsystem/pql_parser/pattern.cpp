#include "pattern.h"

Pattern::Pattern() {}
Pattern::Pattern(PqlToken synonym, PqlToken first, PqlToken second)
    : synonym(synonym),
      first(first),
      second(second) {}
Pattern::Pattern(PqlToken synonym, PqlToken first, PqlToken second, PqlToken third)
    : synonym(synonym),
      first(first),
      second(second),
      third(third) {}

PqlToken Pattern::GetSynonym() {
  return synonym;
}

PqlToken Pattern::GetFirst() {
  return first;
}

PqlToken Pattern::GetSecond() {
  return second;
}

PqlToken Pattern::GetThird() {
  return third;
}