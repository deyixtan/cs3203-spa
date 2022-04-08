#ifndef FOLLOWST_CLAUSE_H_
#define FOLLOWST_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/pkb/pkb.h"

namespace pql {

class FollowsTClause : public Clause {
 public:
  FollowsTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
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
  Table HandleSynonymInteger();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardInteger();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerInteger();
};

}

#endif //FOLLOWST_CLAUSE_H_
