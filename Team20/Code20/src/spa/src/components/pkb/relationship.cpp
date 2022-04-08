#include "relationship.h"

Relationship::Relationship() {}

void Relationship::AddLowerSet(IDENT stmt, IDENT_SET set) {
  upper_to_lower_set.insert({stmt, set});
}

void Relationship::AddUpperSet(IDENT stmt, IDENT_SET set) {
  lower_to_upper_set.insert({stmt, set});
}

void Relationship::AddPair(IDENT stmt1, IDENT stmt2) {
  pair_vector.push_back({stmt1, stmt2});
}

IDENT_SET_MAP Relationship::GetUpperToLowerSet() {
  return upper_to_lower_set;
}

IDENT_SET_MAP Relationship::GetLowerToUpperSet() {
  return lower_to_upper_set;
}

IDENT_PAIR_VECTOR Relationship::GetPairVector() {
  return pair_vector;
}