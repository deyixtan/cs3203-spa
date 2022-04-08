#ifndef WITH_CLAUSE_H
#define WITH_CLAUSE_H

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "../utils.h"

namespace pql {

class WithClause : public Clause {
 public:
  WithClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
             const PqlToken &first_arg,
             const PqlToken &second_arg,
             const PkbPtr &pkb);
  Table Execute() override;
 private:
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  Table HandleAttributeAttribute();
  Table HandleAttributeInteger();
  Table HandleAttributeIdent();
  Table HandleIntegerAttribute();
  Table HandleIntegerInteger();
  Table HandleIdentAttribute();
  Table HandleIdentIdent();
  std::unordered_set<std::string> HandleSetIntersectionSingleColumn(std::unordered_set<std::string>,
                                                                    std::unordered_set<std::string>);
  std::unordered_set<std::pair<std::string, std::string>,
                     pair_hash> HandleSetIntersection(std::unordered_set<std::string>, std::unordered_set<std::string>);
};

}

#endif // WITH_CLAUSE_H
