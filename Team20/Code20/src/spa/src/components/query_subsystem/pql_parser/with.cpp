#include "with.h"

using namespace pql;

With::With() {}
With::With(PqlToken first, PqlToken second): first(first), second(second) {}

PqlToken With::GetFirst() {
  return first;
}

PqlToken With::GetSecond() {
  return second;
}
