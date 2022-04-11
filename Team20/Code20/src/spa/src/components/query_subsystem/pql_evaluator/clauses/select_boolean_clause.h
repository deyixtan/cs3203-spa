#ifndef SELECT_BOOLEAN_CLAUSE_H_
#define SELECT_BOOLEAN_CLAUSE_H_

#include "components/query_subsystem/pql_evaluator/clauses/clause.h"
#include "components/query_subsystem/pql_evaluator/clause_util.h"

namespace pql {

class SelectBooleanClause : public Clause {
 public:
  SelectBooleanClause() = default;;
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
};

}

#endif //SELECT_BOOLEAN_CLAUSE_H_
