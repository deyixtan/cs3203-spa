#include "calls_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

CallsClause::CallsClause(const PqlToken &first_arg, const PqlToken &second_arg, PKB *pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table CallsClause::Execute() {
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

Table CallsClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetCallStore()->GetAllCalls();
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table CallsClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetCallStore()->GetAllCalls();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }

  return {first_arg.value, single_constraints};
}

Table CallsClause::HandleSynonymIdent() {
  auto single_constraints = pkb->GetCallStore()->GetCallersOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table CallsClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetCallStore()->GetAllCalls();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }

  return {second_arg.value, single_constraints};
}

Table CallsClause::HandleWildcardWildcard() {
  bool is_empty = pkb->GetCallStore()->GetAllCalls().empty();
  return ConstructEmptyTable(is_empty);
}

Table CallsClause::HandleWildcardIdent() {
  bool is_empty = pkb->GetCallStore()->GetCallersOf(second_arg.value).empty();
  return ConstructEmptyTable(is_empty);
}

Table CallsClause::HandleIdentSynonym() {
  auto single_constraints = pkb->GetCallStore()->GetCalleesOf(PROC, first_arg.value); //TODO: Fix StmtType
  return {second_arg.value, single_constraints};
}

Table CallsClause::HandleIdentWildcard() {
  bool is_empty = pkb->GetCallStore()->GetCalleesOf(PROC, first_arg.value).empty(); //TODO: Fix StmtType
  return ConstructEmptyTable(is_empty);
}

Table CallsClause::HandleIdentIdent() {
  bool is_empty =
      !pkb->GetCallStore()->IsCallsPairValid(std::pair<std::string, std::string>(first_arg.value, second_arg.value));
  return ConstructEmptyTable(is_empty);
}

}
