#ifndef PKB_RELATIONSHIP_H
#define PKB_RELATIONSHIP_H

#include "pkb_declarations.h"

#include <string>
#include <unordered_set>
#include <unordered_map>

class PkbRelationship {
 private:
  IDENT_SET_MAP upper_to_lower_set;
  IDENT_SET_MAP lower_to_upper_set;
  IDENT_PAIR_VECTOR pair_vector;

 public:
  PkbRelationship();
  void AddLowerSet(IDENT stmt1, IDENT stmt2);
  void AddUpperSet(IDENT stmt1, IDENT stmt2);
  void AddPair(IDENT stmt1, IDENT stmt2);
  [[nodiscard]] IDENT_SET_MAP GetUpperToLowerSet();
  [[nodiscard]] IDENT_SET_MAP GetLowerToUpperSet();
  [[nodiscard]] IDENT_PAIR_VECTOR GetPairVector();
};

#endif //PKB_RELATIONSHIP_H
