#ifndef MODIFIESP_CLAUSE_H_
#define MODIFIESP_CLAUSE_H_

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/declaration.h"

namespace pql {

class ModifiesPClause : public Clause {
 public:
  ModifiesPClause(const std::vector<Declaration> &declarations,
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
  Table HandleIdentSynonym();
  Table HandleIdentWildcard();
  Table HandleIdentIdent();

};

}

#endif //MODIFIESP_CLAUSE_H_
