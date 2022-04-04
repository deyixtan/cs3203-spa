#include "relationship.h"

Relationship::Relationship() {}
Relationship::Relationship(PqlToken rel_ref) : rel_ref_(rel_ref) {}
Relationship::Relationship(PqlToken rel_ref, PqlToken first, PqlToken second): rel_ref_(rel_ref), first_(first), second_(second) {}

PqlToken Relationship::GetRelRef() {
  return rel_ref_;
}

PqlToken Relationship::GetFirst() {
  return first_;
}

PqlToken Relationship::GetSecond() {
  return second_;
}

void Relationship::SetFirst(PqlToken token) {
  first_ = token;
}

void Relationship::SetSecond(PqlToken token) {
  second_ = token;
}