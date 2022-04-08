#ifndef AFFECTS_CLAUSE_H_
#define AFFECTS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/utils.h"
#include "components/pkb/pkb.h"

namespace pql {

class AffectsClause : public Clause {
 public:
  AffectsClause(const PqlToken &first_arg,
                const PqlToken &second_arg,
                const PkbPtr &pkb);
  Table Execute() override;
 private:
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymInteger();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardInteger();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerInteger();
};

}

#endif //AFFECTS_CLAUSE_H_
