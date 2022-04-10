#ifndef SELECT_BOOLEAN_CLAUSE_H_
#define SELECT_BOOLEAN_CLAUSE_H_

#include "clause.h"
#include "clause_util.h"

namespace pql {

class SelectBooleanClause : public Clause {
 public:
  SelectBooleanClause() = default;;
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
};

}

#endif //SELECT_BOOLEAN_CLAUSE_H_
