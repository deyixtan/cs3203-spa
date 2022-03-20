#include "callst_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

CallsTClause::CallsTClause(const PqlToken &first_arg, const PqlToken &second_arg, PKB *pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table CallsTClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    return HandleSynonymIdent();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgIdent(second_arg)) {
    return HandleWildcardIdent();
  } else if (IsArgIdent(first_arg) && IsArgSynonym(second_arg)) {
    return HandleIdentSynonym();
  } else if (IsArgIdent(first_arg) && IsArgWildcard(second_arg)) {
    return HandleIdentWildcard();
  } else if (IsArgIdent(first_arg) && IsArgIdent(second_arg)) {
    return HandleIdentIdent();
  } else {
    // EXCEPTIONAL SITUATION
    return {};
  }
}

Table CallsTClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetCallStore()->GetAllCallsStar();
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table CallsTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetCallStore()->GetAllCallsStar();
  std::unordered_set<std::string> single_constraints;
  for (const auto& pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }

  return {first_arg.value, single_constraints};
}

Table CallsTClause::HandleSynonymIdent() {
  auto single_constraints = pkb->GetCallStore()->GetCallersStarOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table CallsTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetCallStore()->GetAllCallsStar();
  std::unordered_set<std::string> single_constraints;
  for (const auto& pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }

  return {second_arg.value, single_constraints};
}

Table CallsTClause::HandleWildcardWildcard() {
  bool is_empty = pkb->GetCallStore()->GetAllCallsStar().empty();
  Table table;
  if (is_empty) {
    table.ToggleFalseClause();
  }
  return table;
}

Table CallsTClause::HandleWildcardIdent() {
  bool is_empty = pkb->GetCallStore()->GetCallersStarOf(second_arg.value).empty();
  Table table;
  if (is_empty) {
    table.ToggleFalseClause();
  }
  return table;
}

Table CallsTClause::HandleIdentSynonym() {
  auto single_constraints = pkb->GetCallStore()->GetCalleesStarOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table CallsTClause::HandleIdentWildcard() {
  bool is_empty = pkb->GetCallStore()->GetCalleesStarOf(first_arg.value).empty();
  Table table;
  if (is_empty) {
    table.ToggleFalseClause();
  }
  return table;
}

Table CallsTClause::HandleIdentIdent() {
  bool is_empty = !pkb->GetCallStore()->IsCallsStarPairExists(first_arg.value, second_arg.value);
  Table table;
  if (is_empty) {
    table.ToggleFalseClause();
  }
  return table;
}

}
