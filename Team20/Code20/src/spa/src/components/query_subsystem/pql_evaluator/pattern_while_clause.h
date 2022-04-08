#ifndef PATTERN_WHILE_CLAUSE_H_
#define PATTERN_WHILE_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class PatternWhileClause : public Clause {
 public:
  PatternWhileClause(const std::string &while_synonym,
                     const PqlToken &first_arg,
                     const PkbPtr &pkb);

  Table Execute() override;
 private:
  std::string while_synonym;
  PqlToken first_arg;
  const PkbPtr &pkb;
  Table HandleSynonym();
  Table HandleWildcard();
  Table HandleIdent();
};

}

#endif //PATTERN_WHILE_CLAUSE_H_
