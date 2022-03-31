#include "affectst_clause.h"
#include "clause_util.h"
#include "components/pkb/stores/affect_store/affect_session.h"

namespace pql {

using namespace clause_util;

AffectsTClause::AffectsTClause(const PqlToken &first_arg,
                               const PqlToken &second_arg,
                               PKB *pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table AffectsTClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Affects*(a1, a2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Affects*(a, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Affects*(a, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Affects*(_, a)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Affects*(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Affects*(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Affects*(1, a)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Affects*(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Affects*(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
}

Table AffectsTClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table AffectsTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table AffectsTClause::HandleSynonymInteger() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table AffectsTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table AffectsTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs().empty();
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table AffectsTClause::HandleWildcardInteger() {
  auto is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsOf(second_arg.value).empty();
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table AffectsTClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectedOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table AffectsTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectedOf(first_arg.value).empty();
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table AffectsTClause::HandleIntegerInteger() {
  auto is_false_clause =
      pkb->GetAffectStore()->GetAffectSession()->DoesAffectExists({first_arg.value, second_arg.value});
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

}
