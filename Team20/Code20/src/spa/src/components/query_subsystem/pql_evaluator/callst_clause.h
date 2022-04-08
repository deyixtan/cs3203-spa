#ifndef CALLST_CLAUSE_H_
#define CALLST_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class CallsTClause : public Clause {
 public:
  CallsTClause(const PqlToken &first_arg,
               const PqlToken &second_arg,
               const PkbPtr &pkb);
  Table Execute() override;
 private:
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardIdent();
  Table HandleIdentSynonym();
  Table HandleIdentWildcard();
  Table HandleIdentIdent();
};

}

#endif //CALLST_CLAUSE_H_
