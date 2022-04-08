#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "common.h"
#include "../../utils/pair_hash.h"

#include <string>
#include <unordered_set>
#include <unordered_map>

class Relationship {
 private:
  IDENT_SET_MAP upper_to_lower_set;
  IDENT_SET_MAP lower_to_upper_set;
  IDENT_PAIR_VECTOR pair_vector;

 public:
  Relationship();
  void AddLowerSet(IDENT stmt, IDENT_SET set);
  void AddUpperSet(IDENT stmt, IDENT_SET set);
  void AddPair(IDENT stmt1, IDENT stmt2);
  [[nodiscard]] IDENT_SET_MAP GetUpperToLowerSet();
  [[nodiscard]] IDENT_SET_MAP GetLowerToUpperSet();
  [[nodiscard]] IDENT_PAIR_VECTOR GetPairVector();
};

#endif //RELATIONSHIP_H
