#include "pattern_if_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

PatternIfClause::PatternIfClause(const std::string &if_synonym, const PqlToken &first_arg, const PkbPtr &pkb)
    : if_synonym(
    if_synonym), first_arg(first_arg), pkb(pkb) {}

Table PatternIfClause::Execute() {
  return (this->*execute_handler.at(first_arg.type))();
}

Table PatternIfClause::HandleSynonym() {
  auto pair_constraints = pkb->GetPatternStore()->GetIfWithPatternSynonym();
  return {if_synonym, first_arg.value, pair_constraints};
}

Table PatternIfClause::HandleWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetIfWithPattern("_");
  return {if_synonym, single_constraints};
}

Table PatternIfClause::HandleIdent() {
  auto single_constraints = pkb->GetPatternStore()->GetIfWithPattern(first_arg.value);
  return {if_synonym, single_constraints};
}

bool PatternIfClause::ExecuteBool() {
  return (this->*execute_bool_handler.at(first_arg.type))();
}

bool PatternIfClause::HandleSynonymBool() {
  auto pair_constraints = pkb->GetPatternStore()->GetIfWithPatternSynonym();
  return pair_constraints.empty();
}

bool PatternIfClause::HandleWildcardBool() {
  auto single_constraints = pkb->GetPatternStore()->GetIfWithPattern("_");
  return single_constraints.empty();
}

bool PatternIfClause::HandleIdentBool() {
  auto single_constraints = pkb->GetPatternStore()->GetIfWithPattern(first_arg.value);
  return single_constraints.empty();
}

std::set<std::string> PatternIfClause::GetSynonyms() {
  std::set<std::string> synonyms;
  synonyms.emplace(if_synonym);
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }

  return synonyms;
}

size_t PatternIfClause::GetSynonymsSize() {
  size_t size = 1;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  return size;
}

}
