#include "with_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;
WithClause::WithClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
           const PqlToken &first_arg,
           const PqlToken &second_arg,
           PKB *pkb) : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}
Table WithClause::Execute() {
  // TODO
}

}