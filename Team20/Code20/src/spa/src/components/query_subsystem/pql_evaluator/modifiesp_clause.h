#ifndef MODIFIESP_CLAUSE_H_
#define MODIFIESP_CLAUSE_H_

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace pql {

class ModifiesPClause : public Clause {
 public:
  ModifiesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                  const PqlToken &first_arg,
                  const PqlToken &second_arg,
                  PKB *pkb);
  Table Execute() override;
 private:
  std::unordered_map<std::string, DesignEntityType> declarations;
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
