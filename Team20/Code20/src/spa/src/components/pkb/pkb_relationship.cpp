#include "pkb_relationship.h"

PkbRelationship::PkbRelationship() {}

void PkbRelationship::AddLowerSet(IDENT stmt, IDENT_SET set) {
  upper_to_lower_set.insert({stmt, set});
}

void PkbRelationship::AddUpperSet(IDENT stmt, IDENT_SET set) {
  lower_to_upper_set.insert({stmt, set});
}

void PkbRelationship::AddPair(IDENT stmt1, IDENT stmt2) {
  pair_vector.push_back({stmt1, stmt2});
}

IDENT_SET_MAP PkbRelationship::GetUpperToLowerSet() {
  return upper_to_lower_set;
}

IDENT_SET_MAP PkbRelationship::GetLowerToUpperSet() {
  return lower_to_upper_set;
}

IDENT_PAIR_VECTOR PkbRelationship::GetPairVector() {
  return pair_vector;
}