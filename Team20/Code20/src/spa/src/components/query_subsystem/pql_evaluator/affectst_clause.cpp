#include "affectst_clause.h"
#include "clause_util.h"
#include "components/pkb/stores/affect_store/affect_session.h"

namespace pql {

using namespace clause_util;

AffectsTClause::AffectsTClause(const PqlToken &first_arg,
                               const PqlToken &second_arg,
                               const PkbPtr &pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table AffectsTClause::Execute() {
  if (!pkb->GetAffectStore()->DoesAffectSessionExist()) {
    pkb->GetAffectStore()->ComputeAffectSession();
  }

  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table AffectsTClause::HandleSynonymSynonym() {
  // TODO: same synonym non-pair
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs();
  if (first_arg.value == second_arg.value) {
    pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsSameSynPairs();
  }
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table AffectsTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table AffectsTClause::HandleSynonymInteger() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table AffectsTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table AffectsTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs().empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsTClause::HandleWildcardInteger() {
  auto is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarOf(second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsTClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectedStarOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table AffectsTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectedStarOf(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsTClause::HandleIntegerInteger() {
  auto is_false_clause =
      !pkb->GetAffectStore()->GetAffectSession()->DoesAffectStarExists({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool AffectsTClause::ExecuteBool() {
  if (!pkb->GetAffectStore()->DoesAffectSessionExist()) {
    pkb->GetAffectStore()->ComputeAffectSession();
  }

  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool AffectsTClause::HandleSynonymSynonymBool() {
  // TODO: same synonym non-pair
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs();
  if (first_arg.value == second_arg.value) {
    pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsSameSynPairs();
  }
  return pair_constraints.empty();
}

bool AffectsTClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return pair_constraints.empty();
}

bool AffectsTClause::HandleSynonymIntegerBool() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarOf(second_arg.value);
  return single_constraints.empty();
}

bool AffectsTClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return pair_constraints.empty();
}

bool AffectsTClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarPairs().empty();
  return is_false_clause;
}

bool AffectsTClause::HandleWildcardIntegerBool() {
  auto is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectsStarOf(second_arg.value).empty();
  return is_false_clause;
}

bool AffectsTClause::HandleIntegerSynonymBool() {
  auto single_constraints = pkb->GetAffectStore()->GetAffectSession()->GetAffectedStarOf(first_arg.value);
  return single_constraints.empty();
}

bool AffectsTClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetAffectStore()->GetAffectSession()->GetAffectedStarOf(first_arg.value).empty();
  return is_false_clause;
}

bool AffectsTClause::HandleIntegerIntegerBool() {
  auto is_false_clause =
      !pkb->GetAffectStore()->GetAffectSession()->DoesAffectStarExists({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> AffectsTClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t AffectsTClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t AffectsTClause::GetWeight() {
  return weight;
}

}
