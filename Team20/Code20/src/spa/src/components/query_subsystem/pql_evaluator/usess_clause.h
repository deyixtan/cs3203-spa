#ifndef USESS_CLAUSE_H_
#define USESS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/declaration.h"
#include "components/pkb/stores/store.h"
#include "components/pkb/pkb.h"

namespace pql {

class UsesSClause : public Clause {
 public:
  UsesSClause(const std::vector<Declaration> &declarations,
              const PqlToken &first_arg,
              const PqlToken &second_arg,
              PKB *pkb);
  Table Execute() override;
 private:
  std::vector<Declaration> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  PKB *pkb;
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerIdent();
};

}

#endif //USESS_CLAUSE_H_
