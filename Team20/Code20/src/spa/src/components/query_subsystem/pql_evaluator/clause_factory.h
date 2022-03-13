#ifndef CLAUSE_FACTORY_H_
#define CLAUSE_FACTORY_H_

#include "clause.h"
#include "components/query_subsystem/pql_parser/relationship.h"
#include "components/query_subsystem/pql_parser/pattern.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/pkb/pkb.h"

#include <memory>

namespace pql {

class ClauseFactory {
 public:
  ClauseFactory() = default;
  static std::unique_ptr<Clause> Create(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations, PKB *pkb);
  static std::unique_ptr<Clause> Create(Pattern pattern, PKB *pkb);
  static std::unique_ptr<Clause> Create(PqlToken selected_synonym, std::unordered_map<std::string, DesignEntityType> declarations, PKB *pkb);

 private:
  static PKB *pkb;
};

}

#endif //CLAUSE_FACTORY_H_
