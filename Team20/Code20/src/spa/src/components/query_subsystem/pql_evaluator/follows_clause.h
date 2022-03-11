#ifndef FOLLOWS_CLAUSE_H_
#define FOLLOWS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_parser/declaration.h"
#include "components/pkb/pkb.h"

namespace pql {

class FollowsClause : public Clause {
 public:
  FollowsClause(const std::vector<Declaration> &declarations,
                const PqlToken &first_arg,
                const PqlToken &second_arg,
                PKB *pkb);
  Table Execute() override;
 private:
  std::vector<Declaration> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  PKB *pkb;
  Table HandleSynonymSynoym();
  Table HandleSynonymWildcard();
  Table HandleSynonymInteger();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardInteger();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerInteger();
};

}

#endif //FOLLOWS_CLAUSE_H_
