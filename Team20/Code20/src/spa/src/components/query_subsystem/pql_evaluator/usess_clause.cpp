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
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
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

bool UsesSClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool UsesSClause::HandleSynonymSynonymBool() {
  auto pair_constraints =
      pkb->GetUsesStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  return pair_constraints.empty();
}

bool UsesSClause::HandleSynonymWildcardBool() {
  auto pair_constraints =
      pkb->GetUsesStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool UsesSClause::HandleSynonymIdentBool() {
  auto single_constraints =
      pkb->GetUsesStore()->GetStmtUsedByVar(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                            second_arg.value);
  return single_constraints.empty();
}

bool UsesSClause::HandleIntegerSynonymBool() {
  auto single_constraints = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value);
  return single_constraints.empty();
}

bool UsesSClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value).empty();
  return is_false_clause;
}

bool UsesSClause::HandleIntegerIdentBool() {
  bool is_false_clause = !pkb->GetUsesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> UsesSClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t UsesSClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t UsesSClause::GetWeight() {
  return weight;
}

}