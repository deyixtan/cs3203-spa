#include "usesp_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

UsesPClause::UsesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                         const PqlToken &first_arg_,
                         const PqlToken &second_arg_,
                         const PkbPtr &pkb_)
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
  auto pair_constraints = pkb->GetUsesStore()->GetAllUsesStmt(PROC);
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table UsesPClause::HandleSynonymWildcard() {
  auto single_constraints = pkb->GetUsesStore()->GetAllProcUsing();
  return {first_arg.value, single_constraints};
}

Table UsesPClause::HandleSynonymIdent() {
  auto single_constraints = pkb->GetUsesStore()->GetStmtUsedByVar(PROC, second_arg.value);
  return {first_arg.value, single_constraints};
}

Table UsesPClause::HandleIdentSynonym() {
  auto single_constraints = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table UsesPClause::HandleIdentWildcard() {
  bool is_false_clause = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table UsesPClause::HandleIdentIdent() {
  bool is_false_clause = !pkb->GetUsesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

}