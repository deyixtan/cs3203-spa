#include "pattern_while_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

PatternWhileClause::PatternWhileClause(const std::string &while_synonym, const PqlToken &first_arg, PKB *pkb)
    : while_synonym(while_synonym), first_arg(first_arg), pkb(pkb) {}

Table PatternWhileClause::Execute() {
  if (IsArgSynonym(first_arg)) {
    return HandleSynonym();
  } else if (IsArgWildcard(first_arg)) {
    return HandleWildcard();
  } else if (IsArgIdent(first_arg)) {
    return HandleIdent();
  } else {
    // no type safety because of PqlTokenType
    // should refactor
    return {};
  }
}

Table PatternWhileClause::HandleSynonym() {
  // TODO: wait for Jaryl to implement
}

Table PatternWhileClause::HandleWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetWhileWithPattern("_");
  return {while_synonym, single_constraints};
}

Table PatternWhileClause::HandleIdent() {
  auto single_constraints = pkb->GetPatternStore()->GetWhileWithPattern(first_arg.value);
  return {while_synonym, single_constraints};
}

}
