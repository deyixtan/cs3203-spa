#ifndef SELECT_TUPLE_CLAUSE_H_
#define SELECT_TUPLE_CLAUSE_H_

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/utils.h"

namespace pql {

class SelectTupleClause : public Clause {
 public:
  SelectTupleClause(std::vector<PqlToken> &tuple,
                    const std::unordered_map<std::string, DesignEntityType> &declarations,
                    PKB *pkb);
  Table Execute() override;
 private:
  std::vector<PqlToken> tuple;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PKB *pkb;
};

}

#endif //SELECT_TUPLE_CLAUSE_H_
