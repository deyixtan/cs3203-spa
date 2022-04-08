#ifndef MODIFIESS_CLAUSE_H_
#define MODIFIESS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/pkb/stores/store.h"
#include "components/pkb/pkb.h"

namespace pql {

class ModifiesSClause : public Clause {
 public:
  ModifiesSClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                  const PqlToken &first_arg,
                  const PqlToken &second_arg,
                  const PkbPtr &pkb);
  Table Execute() override;
 private:
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerIdent();
};

}

#endif //MODIFIESS_CLAUSE_H_
