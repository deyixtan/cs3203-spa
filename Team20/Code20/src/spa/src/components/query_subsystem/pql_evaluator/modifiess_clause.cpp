#include "modifiess_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

ModifiesSClause::ModifiesSClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                                 const PqlToken &first_arg_,
                                 const PqlToken &second_arg_,
                                 const PkbPtr &pkb_) :
    declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table ModifiesSClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table ModifiesSClause::HandleSynonymSynonym() {
  auto pair_constraints =
      pkb->GetModifiesStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table ModifiesSClause::HandleSynonymWildcard() {
  auto pair_constraints =
      pkb->GetModifiesStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table ModifiesSClause::HandleSynonymIdent() {
  auto single_constraints =
      pkb->GetModifiesStore()->GetStmtModByVar(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                               second_arg.value);
  return {first_arg.value, single_constraints};
}

Table ModifiesSClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table ModifiesSClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table ModifiesSClause::HandleIntegerIdent() {
  bool is_false_clause = !pkb->GetModifiesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool ModifiesSClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool ModifiesSClause::HandleSynonymSynonymBool() {
  auto pair_constraints =
      pkb->GetModifiesStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  return pair_constraints.empty();
}

bool ModifiesSClause::HandleSynonymWildcardBool() {
  auto pair_constraints =
      pkb->GetModifiesStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool ModifiesSClause::HandleSynonymIdentBool() {
  auto single_constraints =
      pkb->GetModifiesStore()->GetStmtModByVar(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                               second_arg.value);
  return single_constraints.empty();
}

bool ModifiesSClause::HandleIntegerSynonymBool() {
  auto single_constraints = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value);
  return single_constraints.empty();
}

bool ModifiesSClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value).empty();
  return is_false_clause;
}

bool ModifiesSClause::HandleIntegerIdentBool() {
  bool is_false_clause = !pkb->GetModifiesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> ModifiesSClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t ModifiesSClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}
}
