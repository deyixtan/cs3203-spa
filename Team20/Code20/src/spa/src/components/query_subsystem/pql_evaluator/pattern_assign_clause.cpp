#include "pattern_assign_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

PatternAssignClause::PatternAssignClause(const std::string &assign_synonym,
                                         const PqlToken &first_arg,
                                         const PqlToken &second_arg,
                                         PKB *pkb)
    : assign_synonym(assign_synonym), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table PatternAssignClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // pattern a(v, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgExactMatch(second_arg)) {
    // pattern a(v, "x")
    return HandleSynonymExact();
  } else if (IsArgSynonym(first_arg) && IsArgPartialMatch(second_arg)) {
    // pattern a(v, _"x"_)
    return HandleSynonymPartial();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // pattern a(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgExactMatch(second_arg)) {
    // pattern a(_, "x")
    return HandleWildcardExact();
  } else if (IsArgWildcard(first_arg) && IsArgPartialMatch(second_arg)) {
    // pattern a(_, _"x"_)
    return HandleWildcardPartial();
  } else if (IsArgIdent(first_arg) && IsArgWildcard(second_arg)) {
    // pattern a("x", _)
    return HandleIdentWildcard();
  } else if (IsArgIdent(first_arg) && IsArgExactMatch(second_arg)) {
    // pattern a("x", "y")
    return HandleIdentExact();
  } else if (IsArgIdent(first_arg) && IsArgPartialMatch(second_arg)) {
    // pattern a("x", _"y"_)
    return HandleIdentPartial();
  } else {
    // no type safety because of PqlTokenType
    return {};
  }
}

Table PatternAssignClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonym("_");
  return {assign_synonym, first_arg.value, pair_constraints};
}

Table PatternAssignClause::HandleSynonymExact() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonym(second_arg.value);
  return {assign_synonym, first_arg.value, pair_constraints};
}

Table PatternAssignClause::HandleSynonymPartial() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonym(second_arg.value);
  return {assign_synonym, first_arg.value, pair_constraints};
}

Table PatternAssignClause::HandleWildcardWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPattern("_", "_");
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleWildcardExact() {
  // need to strip quotes?
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPattern("_", second_arg.value);
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleWildcardPartial() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPattern("_", second_arg.value);
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleIdentWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPattern(GetIdentWithoutQuotes(first_arg.value), "_");
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleIdentExact() {
  auto single_constraints =
      pkb->GetPatternStore()->GetStmtWithPattern(GetIdentWithoutQuotes(first_arg.value), second_arg.value);
  return {assign_synonym, single_constraints}
}

Table PatternAssignClause::HandleIdentPartial() {
  auto single_constraints =
      pkb->GetPatternStore()->GetStmtWithPattern(GetIdentWithoutQuotes(first_arg.value), second_arg.value);
  return {assign_synonym, single_constraints};
}

}