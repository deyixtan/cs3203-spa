#include "relationship.h"

Relationship::Relationship() {}
Relationship::Relationship(PqlToken rel_ref) : rel_ref(rel_ref) {}
Relationship::Relationship(PqlToken rel_ref, PqlToken first, PqlToken second): rel_ref(rel_ref), first(first), second(second) {}

PqlToken Relationship::GetRelRef() {
  return rel_ref;
}

PqlToken Relationship::GetFirst() {
  return first;
}

PqlToken Relationship::GetSecond() {
  return second;
}

void Relationship::SetFirst(PqlToken token) {
  first = token;
}

void Relationship::SetSecond(PqlToken token) {
  second = token;
}