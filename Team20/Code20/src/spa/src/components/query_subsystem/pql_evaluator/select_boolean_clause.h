#ifndef SELECT_BOOLEAN_CLAUSE_H_
#define SELECT_BOOLEAN_CLAUSE_H_

#include "clause.h"

namespace pql {

class SelectBooleanClause : public Clause {
 public:
  SelectBooleanClause() = default;;
  Table Execute() override;
};

}

#endif //SELECT_BOOLEAN_CLAUSE_H_
