#include "relationship.h"

Relationship::Relationship() {}

IDENT_SET_MAP Relationship::GetUpperToLowerSet() {
  return upper_to_lower_set;
}

IDENT_SET_MAP Relationship::GetLowerToUpperSet() {
  return lower_to_upper_set;
}

IDENT_PAIR_SET Relationship::GetPairSet() {
  return pair_set;
}