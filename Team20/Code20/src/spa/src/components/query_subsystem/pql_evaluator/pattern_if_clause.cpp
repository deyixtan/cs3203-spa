#include "pattern_if_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

PatternIfClause::PatternIfClause(const std::string &if_synonym, const PqlToken &first_arg, PKB *pkb) : if_synonym(
    if_synonym), first_arg(first_arg), pkb(pkb) {}

Table PatternIfClause::Execute() {
  if (IsArgSynonym(first_arg)) {
    // pattern ifs(v, _, _)
    return HandleSynonym();
  } else if (IsArgWildcard(first_arg)) {
    // pattern ifs(_, _, _)
    return HandleWildcard();
  } else if (IsArgIdent(first_arg)) {
    // pattern ifs("x", _, _)
    return HandleIdent();
  } else {
    // no type safety because of PqlTokenType
    // should refactor
    return {};
  }
}

Table PatternIfClause::HandleSynonym() {
  auto pair_constraints = pkb->GetPatternStore()->GetIfWithPatternSynonym();
  return {if_synonym, first_arg.value, pair_constraints};
}

Table PatternIfClause::HandleWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetIfWithPattern("_");
  return {if_synonym, single_constraints};
}

Table PatternIfClause::HandleIdent() {
  auto single_constraints = pkb->GetPatternStore()->GetIfWithPattern(first_arg.value);
  return {if_synonym, single_constraints};
}

}
