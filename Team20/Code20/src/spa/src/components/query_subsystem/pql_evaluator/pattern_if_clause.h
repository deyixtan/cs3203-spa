#ifndef PATTERN_IF_CLAUSE_H_
#define PATTERN_IF_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class PatternIfClause : public Clause {
 public:
  PatternIfClause(const std::string &if_synonym,
                  const PqlToken &first_arg,
                  const PkbPtr &pkb);
  Table Execute() override;
 private:
  std::string if_synonym;
  PqlToken first_arg;
  const PkbPtr &pkb;
  Table HandleSynonym();
  Table HandleWildcard();
  Table HandleIdent();
};

}

#endif //PATTERN_IF_CLAUSE_H_
