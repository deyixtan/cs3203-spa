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
  UsesSClause(const std::vector<Declaration> &declarations, const PqlToken &first_arg, const PqlToken &second_arg, PKB* pkb);
  Table Execute() override;
 private:
  std::vector<Declaration> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  PKB* pkb;
  static bool IsArgSynonym(const PqlToken &arg);
  static bool IsArgWildcard(const PqlToken &arg);
  static bool IsArgIdent(const PqlToken &arg);
  static bool IsArgInteger(const PqlToken &arg);
  PqlTokenType GetSynonymDesignEntity(const PqlToken &arg);
  StmtType GetStmtType(const PqlTokenType &design_entity);
  std::string GetIdentWithoutQuotes(const std::string& ident);
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerIdent();
};

}

#endif //USESS_CLAUSE_H_
