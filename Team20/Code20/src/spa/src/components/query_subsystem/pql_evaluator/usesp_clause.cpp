#include "usesp_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

UsesPClause::UsesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                         const PqlToken &first_arg_,
                         const PqlToken &second_arg_,
                         PKB *pkb_)
    : declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table pql::UsesPClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // UsesP(p, v)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // UsesP(p, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    // UsesP(p, "x")
    return HandleSynonymIdent();
  } else if (IsArgIdent(first_arg) && IsArgSynonym(second_arg)) {
    // UsesP("main", v)
    return HandleIdentSynonym();
  } else if (IsArgIdent(first_arg) && IsArgWildcard(second_arg)) {
    // UsesP("main", _)
    return HandleIdentWildcard();
  } else if (IsArgIdent(first_arg) && IsArgIdent(second_arg)) {
    // UsesP("main", "x")
    return HandleIdentIdent();
  }
}

Table UsesPClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetUsageStore()->GetAllProcVar();
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table UsesPClause::HandleSynonymWildcard() {
  auto single_constraints = pkb->GetUsageStore()->GetAllProcUsing();
  return {first_arg.value, single_constraints};
}

Table UsesPClause::HandleSynonymIdent() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  auto single_constraints = pkb->GetUsageStore()->GetProcUsedByVar(ident_without_quotes);
  return {first_arg.value, single_constraints};
}

Table UsesPClause::HandleIdentSynonym() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(first_arg.value);
  auto single_constraints = pkb->GetUsageStore()->GetVarUsedByProc(ident_without_quotes);
  return {second_arg.value, single_constraints};
}

Table UsesPClause::HandleIdentWildcard() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(first_arg.value);
  bool is_empty = pkb->GetUsageStore()->GetVarUsedByProc(ident_without_quotes).empty();
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table UsesPClause::HandleIdentIdent() {
  std::string first_arg_without_quotes = GetIdentWithoutQuotes(first_arg.value);
  std::string second_arg_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  std::pair arg_pair(first_arg_without_quotes, second_arg_without_quotes);
  bool is_empty = pkb->GetUsageStore()->ProcVarExists(arg_pair);
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

}