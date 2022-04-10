#include "pattern_assign_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

PatternAssignClause::PatternAssignClause(const std::string &assign_synonym,
                                         const PqlToken &first_arg,
                                         const PqlToken &second_arg,
                                         const PkbPtr &pkb)
    : assign_synonym(assign_synonym), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table PatternAssignClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table PatternAssignClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonymWildcard();
  return {assign_synonym, first_arg.value, pair_constraints};
}

Table PatternAssignClause::HandleSynonymExact() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonymExact(second_arg.value);
  return {assign_synonym, first_arg.value, pair_constraints};
}

Table PatternAssignClause::HandleSynonymPartial() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonymPartial(second_arg.value);
  return {assign_synonym, first_arg.value, pair_constraints};
}

Table PatternAssignClause::HandleWildcardWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternWildcard("_");
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleWildcardExact() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternExact("_", second_arg.value);
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleWildcardPartial() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternPartial("_", second_arg.value);
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleIdentWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternWildcard(first_arg.value);
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleIdentExact() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternExact(first_arg.value, second_arg.value);
  return {assign_synonym, single_constraints};
}

Table PatternAssignClause::HandleIdentPartial() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternPartial(first_arg.value, second_arg.value);
  return {assign_synonym, single_constraints};
}

bool PatternAssignClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool PatternAssignClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonymWildcard();
  return pair_constraints.empty();
}

bool PatternAssignClause::HandleSynonymExactBool() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonymExact(second_arg.value);
  return pair_constraints.empty();
}

bool PatternAssignClause::HandleSynonymPartialBool() {
  auto pair_constraints = pkb->GetPatternStore()->GetStmtWithPatternSynonymPartial(second_arg.value);
  return pair_constraints.empty();
}

bool PatternAssignClause::HandleWildcardWildcardBool() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternWildcard("_");
  return single_constraints.empty();
}

bool PatternAssignClause::HandleWildcardExactBool() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternExact("_", second_arg.value);
  return single_constraints.empty();
}

bool PatternAssignClause::HandleWildcardPartialBool() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternPartial("_", second_arg.value);
  return single_constraints.empty();
}

bool PatternAssignClause::HandleIdentWildcardBool() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternWildcard(first_arg.value);
  return single_constraints.empty();
}

bool PatternAssignClause::HandleIdentExactBool() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternExact(first_arg.value, second_arg.value);
  return single_constraints.empty();
}

bool PatternAssignClause::HandleIdentPartialBool() {
  auto single_constraints = pkb->GetPatternStore()->GetStmtWithPatternPartial(first_arg.value, second_arg.value);
  return single_constraints.empty();
}

std::set<std::string> PatternAssignClause::GetSynonyms() {
  std::set<std::string> synonyms;
  synonyms.emplace(assign_synonym);
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }

  return synonyms;
}

size_t PatternAssignClause::GetSynonymsSize() {
  size_t size = 1;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  return size;
}

}