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
  static bool IsArgSynonym(const PqlToken &arg);
  static bool IsArgWildcard(const PqlToken &arg);
  static bool IsArgIdent(const PqlToken &arg);
  static bool IsArgInteger(const PqlToken &arg);
  PqlTokenType GetSynonymDesignEntity(const PqlToken &arg);
  StmtType GetStmtType(const PqlTokenType &design_entity);
  std::string GetIdentWithoutQuotes(const std::string &ident);
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIdentSynonym();
  Table HandleIdentWildcard();
  Table HandleIdentIdent();

};

}

#endif //MODIFIESP_CLAUSE_H_
