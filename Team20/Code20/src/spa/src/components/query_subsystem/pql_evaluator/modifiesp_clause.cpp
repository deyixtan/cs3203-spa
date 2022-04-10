#include "modifiesp_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

ModifiesPClause::ModifiesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations_,
                                 const PqlToken &first_arg_,
                                 const PqlToken &second_arg_,
                                 const pkb::PkbPtr &pkb_)
    : declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table pql::ModifiesPClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table ModifiesPClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetModifiesStore()->GetAllModStmt(PROC);
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table ModifiesPClause::HandleSynonymWildcard() {
  auto single_constraints = pkb->GetModifiesStore()->GetAllProcModify();
  return {first_arg.value, single_constraints};
}

Table ModifiesPClause::HandleSynonymIdent() {
  auto single_constraints = pkb->GetModifiesStore()->GetStmtModByVar(PROC, second_arg.value);
  return {first_arg.value, single_constraints};
}

Table ModifiesPClause::HandleIdentSynonym() {
  auto single_constraints = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table ModifiesPClause::HandleIdentWildcard() {
  bool is_false_clause = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table ModifiesPClause::HandleIdentIdent() {
  bool is_false_clause = !pkb->GetModifiesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool ModifiesPClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool ModifiesPClause::HandleSynonymSynonymBool() {
  auto pair_constraints = pkb->GetModifiesStore()->GetAllModStmt(PROC);
  return pair_constraints.empty();
}

bool ModifiesPClause::HandleSynonymWildcardBool() {
  auto single_constraints = pkb->GetModifiesStore()->GetAllProcModify();
  return single_constraints.empty();
}

bool ModifiesPClause::HandleSynonymIdentBool() {
  auto single_constraints = pkb->GetModifiesStore()->GetStmtModByVar(PROC, second_arg.value);
  return single_constraints.empty();
}

bool ModifiesPClause::HandleIdentSynonymBool() {
  auto single_constraints = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value);
  return single_constraints.empty();
}

bool ModifiesPClause::HandleIdentWildcardBool() {
  bool is_false_clause = pkb->GetModifiesStore()->GetVarModByStmt(first_arg.value).empty();
  return is_false_clause;
}

bool ModifiesPClause::HandleIdentIdentBool() {
  bool is_false_clause = !pkb->GetModifiesStore()->IsStmtVarValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> ModifiesPClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t ModifiesPClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t ModifiesPClause::GetWeight() {
  return weight;
}

}
