#include "usesp_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

UsesPClause::UsesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                         const PqlToken &first_arg_,
                         const PqlToken &second_arg_,
                         const pkb::PkbPtr &pkb_)
    : declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table UsesPClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
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

bool UsesPClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool UsesPClause::HandleSynonymSynonymBool() {
  auto pair_constraints = pkb->GetUsesStore()->GetAllUsesStmt(PROC);
  return pair_constraints.empty();
}

bool UsesPClause::HandleSynonymWildcardBool() {
  auto single_constraints = pkb->GetUsesStore()->GetAllProcUsing();
  return single_constraints.empty();
}

bool UsesPClause::HandleSynonymIdentBool() {
  auto single_constraints = pkb->GetUsesStore()->GetStmtUsedByVar(PROC, second_arg.value);
  return single_constraints.empty();
}

bool UsesPClause::HandleIdentSynonymBool() {
  auto single_constraints = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value);
  return single_constraints.empty();
}

bool UsesPClause::HandleIdentWildcardBool() {
  bool is_false_clause = pkb->GetUsesStore()->GetVarUsedByStmt(first_arg.value).empty();
  return is_false_clause;
}

bool UsesPClause::HandleIdentIdentBool() {
  bool is_false_clause = !pkb->GetUsesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> UsesPClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t UsesPClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t UsesPClause::GetWeight() {
  return weight;
}

}