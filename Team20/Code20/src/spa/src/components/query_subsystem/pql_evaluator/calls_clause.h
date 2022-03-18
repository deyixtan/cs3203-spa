#ifndef CALLS_CLAUSE_H_
#define CALLS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class CallsClause : public Clause {
 public:
  CallsClause(const PqlToken &first_arg,
              const PqlToken &second_arg,
              PKB *pkb);
  Table Execute() override;
 private:
  PqlToken first_arg;
  PqlToken second_arg;
  PKB *pkb;
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

#endif //CALLS_CLAUSE_H_
