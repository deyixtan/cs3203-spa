#include "usess_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

UsesSClause::UsesSClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                         const PqlToken &first_arg_,
                         const PqlToken &second_arg_,
                         const PkbPtr &pkb_) :
    declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table UsesSClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // UsesS(s, v)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // UsesS(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    // UsesS(s, "x")
    return HandleSynonymIdent();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // UsesS(1, v)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // UsesS(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgIdent(second_arg)) {
    // UsesS(1, "x")
    return HandleIntegerIdent();
  }
}

Table UsesSClause::HandleSynonymSynonym() {
  auto pair_constraints =
      pkb->GetUsesStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table UsesSClause::HandleSynonymWildcard() {
  auto pair_constraints =
      pkb->GetUsesStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table UsesSClause::HandleSynonymIdent() {
  auto single_constraints =
      pkb->GetUsesStore()->GetStmtUsedByVar(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                            second_arg.value);
  return {first_arg.value, single_constraints};
}

Table UsesSClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table UsesSClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table UsesSClause::HandleIntegerIdent() {
  bool is_false_clause = !pkb->GetUsesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

}