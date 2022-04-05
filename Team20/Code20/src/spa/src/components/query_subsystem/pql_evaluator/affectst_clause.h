#ifndef AFFECTST_CLAUSE_H_
#define AFFECTST_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/utils.h"
#include "components/pkb/pkb.h"

namespace pql {

class AffectsTClause : public Clause {
 public:
  AffectsTClause(const PqlToken &first_arg,
                 const PqlToken &second_arg,
                 PKB *pkb);
  Table Execute() override;
 private:
  PqlToken first_arg;
  PqlToken second_arg;
  PKB *pkb;
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
#endif //AFFECTST_CLAUSE_H_