#ifndef PATTERN_ASSIGN_CLAUSE_H_
#define PATTERN_ASSIGN_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class PatternAssignClause : public Clause {
 public:
  PatternAssignClause(const std::string &assign_synonym,
                      const PqlToken &first_arg,
                      const PqlToken &second_arg,
                      PKB *pkb);

  Table Execute() override;
 private:
  std::string assign_synonym;
  PqlToken first_arg;
  PqlToken second_arg;
  PKB *pkb;
  Table HandleSynonymWildcard();
  Table HandleSynonymPartial();
  Table HandleWildcardWildcard();
  Table HandleWildcardPartial();
  Table HandleIdentWildcard();
  Table HandleIdentPartial();
};

}

#endif //PATTERN_ASSIGN_CLAUSE_H_