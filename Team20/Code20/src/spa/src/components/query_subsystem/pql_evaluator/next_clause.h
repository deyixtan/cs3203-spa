#ifndef NEXT_CLAUSE_H_
#define NEXT_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/utils.h"
#include "components/pkb/pkb.h"

namespace pql {

class NextClause : public Clause {
 public:
  NextClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
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
  Table HandleSynonymInteger();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardInteger();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerInteger();

};

}

#endif //NEXT_CLAUSE_H_
