#include "pkb_relationship.h"

namespace pkb {

PkbRelationship::PkbRelationship() {}

void PkbRelationship::AddLowerSet(IDENT stmt1, IDENT stmt2) {
  if (upper_to_lower_set.find(stmt1) != upper_to_lower_set.end()) {
    upper_to_lower_set.at(stmt1).insert(stmt2);
  } else {
    upper_to_lower_set.insert({stmt1, {stmt2}});
  }
}

void PkbRelationship::AddUpperSet(IDENT stmt1, IDENT stmt2) {
  if (lower_to_upper_set.find(stmt1) != lower_to_upper_set.end()) {
    lower_to_upper_set.at(stmt1).insert(stmt2);
  } else {
    lower_to_upper_set.insert({stmt1, {stmt2}});
  }
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

}
