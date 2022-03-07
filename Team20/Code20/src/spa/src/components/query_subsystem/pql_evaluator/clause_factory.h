#ifndef CLAUSE_FACTORY_H_
#define CLAUSE_FACTORY_H_

#include "clause.h"
#include "components/query_subsystem/pql_parser/relationship.h"
#include "components/query_subsystem/pql_parser/pattern.h"
#include "components/query_subsystem/pql_parser/declaration.h"
#include "components/pkb/pkb.h"

#include <memory>

namespace pql {

class ClauseFactory {
 public:
  ClauseFactory() = default;
  ClauseFactory(PKB *pkb);
  static std::unique_ptr<Clause> Create(Relationship relationship, std::vector<Declaration> declarations);
  static std::unique_ptr<Clause> Create(Pattern pattern, std::vector<Declaration> declarations);
  static std::unique_ptr<Clause> Create(PqlToken selected_synonym, std::vector<Declaration> declarations);

 private:
  static PKB *pkb;
  static bool IsArgInteger(const PqlToken &arg);
  static bool IsArgIdent(const PqlToken &arg);
  static bool IsArgProcSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations);
  static bool IsArgStmtSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations);
  static PqlTokenType GetSynonymDesignEntity(const PqlToken &arg, const std::vector<Declaration> &declarations);
};

}

#endif //CLAUSE_FACTORY_H_
