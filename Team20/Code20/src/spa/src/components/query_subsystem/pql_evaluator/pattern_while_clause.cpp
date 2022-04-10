#include "pattern_while_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

PatternWhileClause::PatternWhileClause(const std::string &while_synonym, const PqlToken &first_arg, const pkb::PkbPtr &pkb)
    : while_synonym(while_synonym), first_arg(first_arg), pkb(pkb) {}

Table PatternWhileClause::Execute() {
  return (this->*execute_handler.at(first_arg.type))();
}

Table PatternWhileClause::HandleSynonym() {
  auto pair_constraints = pkb->GetPatternStore()->GetWhileWithPatternSynonym();
  return {while_synonym, first_arg.value, pair_constraints};
}

Table PatternWhileClause::HandleWildcard() {
  auto single_constraints = pkb->GetPatternStore()->GetWhileWithPattern("_");
  return {while_synonym, single_constraints};
}

Table PatternWhileClause::HandleIdent() {
  auto single_constraints = pkb->GetPatternStore()->GetWhileWithPattern(first_arg.value);
  return {while_synonym, single_constraints};
}

bool PatternWhileClause::ExecuteBool() {
  return (this->*execute_bool_handler.at(first_arg.type))();
}

bool PatternWhileClause::HandleSynonymBool() {
  auto pair_constraints = pkb->GetPatternStore()->GetWhileWithPatternSynonym();
  return pair_constraints.empty();
}

bool PatternWhileClause::HandleWildcardBool() {
  auto single_constraints = pkb->GetPatternStore()->GetWhileWithPattern("_");
  return single_constraints.empty();
}

bool PatternWhileClause::HandleIdentBool() {
  auto single_constraints = pkb->GetPatternStore()->GetWhileWithPattern(first_arg.value);
  return single_constraints.empty();
}

std::set<std::string> PatternWhileClause::GetSynonyms() {
  std::set<std::string> synonyms;
  synonyms.emplace(while_synonym);
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }

  return synonyms;
}

size_t PatternWhileClause::GetSynonymsSize() {
  size_t size = 1;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  return size;
}

size_t PatternWhileClause::GetWeight() {
  return weight;
}

}
