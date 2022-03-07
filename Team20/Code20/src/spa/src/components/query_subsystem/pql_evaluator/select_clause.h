#ifndef SELECT_CLAUSE_H_
#define SELECT_CLAUSE_H_

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/declaration.h"

namespace pql {

class SelectClause : public Clause {
 public:
  SelectClause(const PqlToken &selected_synonym, const std::vector<Declaration> &declarations, PKB *pkb);
  Table Execute() override;
 private:
  PqlToken selected_synonym;
  std::vector<Declaration> declarations;
  PKB *pkb;
  PqlTokenType GetSynonymDesignEntity(const PqlToken &arg);
  StmtType GetStmtType(const PqlTokenType &design_entity);

};

}

#endif //SELECT_CLAUSE_H_
