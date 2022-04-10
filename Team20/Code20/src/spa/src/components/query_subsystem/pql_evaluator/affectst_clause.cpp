#include "affectst_clause.h"
#include "clause_util.h"
#include "components/pkb/stores/affects_store/affects_store.h"

namespace pql {

using namespace clause_util;

AffectsTClause::AffectsTClause(const PqlToken &first_arg,
                               const PqlToken &second_arg,
                               const PkbPtr &pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table AffectsTClause::Execute() {
  if (!pkb->GetAffectsStoreFactory()->DoesAffectsStoreExist()) {
    pkb->GetAffectsStoreFactory()->ComputeAffectsStore();
  }

  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Affects*(a1, a2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Affects*(a, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Affects*(a, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Affects*(_, a)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Affects*(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Affects*(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Affects*(1, a)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Affects*(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Affects*(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
}

Table AffectsTClause::HandleSynonymSynonym() {
  if (first_arg.value == second_arg.value) {
    auto single_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarSameSynSet();
    return {first_arg.value, single_constraints};
  }
  auto pair_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs();
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table AffectsTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table AffectsTClause::HandleSynonymInteger() {
  auto single_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table AffectsTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table AffectsTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs().empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsTClause::HandleWildcardInteger() {
  auto is_false_clause = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarOf(second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsTClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectedStarOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table AffectsTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectedStarOf(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table AffectsTClause::HandleIntegerInteger() {
  auto is_false_clause =
      !pkb->GetAffectsStoreFactory()->GetAffectsStore()->DoesAffectStarExists({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool AffectsTClause::ExecuteBool() {
  if (!pkb->GetAffectsStoreFactory()->DoesAffectsStoreExist()) {
    pkb->GetAffectsStoreFactory()->ComputeAffectsStore();
  }

  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool AffectsTClause::HandleSynonymSynonymBool() {
  if (first_arg.value == second_arg.value) {
    auto single_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarSameSynSet();
    return single_constraints.empty();
  }
  auto pair_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs();
  return pair_constraints.empty();
}

bool AffectsTClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return pair_constraints.empty();
}

bool AffectsTClause::HandleSynonymIntegerBool() {
  auto single_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarOf(second_arg.value);
  return single_constraints.empty();
}

bool AffectsTClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs();
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return pair_constraints.empty();
}

bool AffectsTClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarPairs().empty();
  return is_false_clause;
}

bool AffectsTClause::HandleWildcardIntegerBool() {
  auto is_false_clause = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectsStarOf(second_arg.value).empty();
  return is_false_clause;
}

bool AffectsTClause::HandleIntegerSynonymBool() {
  auto single_constraints = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectedStarOf(first_arg.value);
  return single_constraints.empty();
}

bool AffectsTClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetAffectsStoreFactory()->GetAffectsStore()->GetAffectedStarOf(first_arg.value).empty();
  return is_false_clause;
}

bool AffectsTClause::HandleIntegerIntegerBool() {
  auto is_false_clause =
      !pkb->GetAffectsStoreFactory()->GetAffectsStore()->DoesAffectStarExists({first_arg.value, second_arg.value});
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
