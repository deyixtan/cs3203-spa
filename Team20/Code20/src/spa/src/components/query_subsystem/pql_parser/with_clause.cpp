#include "with_clause.h"

With::With() {}
With::With(PqlToken first, PqlToken second): first_(first), second_(second) {}

PqlToken With::GetFirst() {
  return first_;
}

PqlToken With::GetSecond() {
  return second_;
}
