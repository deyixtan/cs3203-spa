#include "affects_clause.h"
#include "clause_util.h"
#include "components/pkb/stores/affect_store/affect_session.h"

namespace pql {

using namespace clause_util;

AffectsClause::AffectsClause(const PqlToken &first_arg,
                             const PqlToken &second_arg,
                             const PkbPtr &pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table AffectsClause::Execute() {
  if (!pkb->GetAffectStore()->DoesAffectSessionExist()) {
    pkb->GetAffectStore()->ComputeAffectSession();
  }

  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table AffectsClause::HandleSynonymSynonym() {
  // TODO: same synonym non-pair
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  if (first_arg.value==second_arg.value) {
    pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsSameSynPairs();
  }
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table AffectsClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table AffectsClause::HandleSynonymInteger() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table AffectsClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table AffectsClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs().empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsClause::HandleWildcardInteger() {
  auto is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsOf(second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectedOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table AffectsClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectedOf(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsClause::HandleIntegerInteger() {
  auto is_false_clause =
      !pkb->GetAffectStore()->GetAffectSession()->DoesAffectExists({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool AffectsClause::ExecuteBool() {
  if (!pkb->GetAffectStore()->DoesAffectSessionExist()) {
    pkb->GetAffectStore()->ComputeAffectSession();
  }

  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool AffectsClause::HandleSynonymSynonymBool() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  if (first_arg.value==second_arg.value) {
    pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsSameSynPairs();
  }
  return pair_constraints.empty();
}

bool AffectsClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool AffectsClause::HandleSynonymIntegerBool() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsOf(second_arg.value);
  return single_constraints.empty();
}

bool AffectsClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool AffectsClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsPairs().empty();
  return is_false_clause;
}

bool AffectsClause::HandleWildcardIntegerBool() {
  auto is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsOf(second_arg.value).empty();
  return is_false_clause;
}

bool AffectsClause::HandleIntegerSynonymBool() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectedOf(first_arg.value);
  return single_constraints.empty();
}

bool AffectsClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectedOf(first_arg.value).empty();
  return is_false_clause;
}

bool AffectsClause::HandleIntegerIntegerBool() {
  auto is_false_clause =
      !pkb->GetAffectStore()->GetAffectSession()->DoesAffectExists({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> AffectsClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t AffectsClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t AffectsClause::GetWeight() {
  return weight;
}

}
