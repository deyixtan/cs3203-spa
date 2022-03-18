#include "modifiess_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

ModifiesSClause::ModifiesSClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                                 const PqlToken &first_arg_,
                                 const PqlToken &second_arg_,
                                 PKB *pkb_) :
    declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table ModifiesSClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // ModifiesS(s, v)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // ModifiesS(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    // ModifiesS(s, "x")
    return HandleSynonymIdent();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // ModifiesS(1, v)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // ModifiesS(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgIdent(second_arg)) {
    // ModifiesS(1, "x")
    return HandleIntegerIdent();
  }
}

Table ModifiesSClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetModifyStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table ModifiesSClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetModifyStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table ModifiesSClause::HandleSynonymIdent() {
  auto pair_constraints = pkb->GetModifyStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    if (pair_constraint.second==second_arg.value) {
      single_constraints.insert(pair_constraint.first);
    }
  }
  return {first_arg.value, single_constraints};
}

Table ModifiesSClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetModifyStore()->GetVarModByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table ModifiesSClause::HandleIntegerWildcard() {
  bool is_empty = pkb->GetModifyStore()->GetVarModByStmt(first_arg.value).empty();
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table ModifiesSClause::HandleIntegerIdent() {
  std::pair arg_pair(first_arg.value, second_arg.value);
  bool is_empty = !pkb->GetModifyStore()->StmtVarExists(arg_pair);
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

}
