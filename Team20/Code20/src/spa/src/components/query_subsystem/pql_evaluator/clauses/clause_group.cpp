#include "clause_group.h"

namespace pql {

ClauseGroup::ClauseGroup() : synonyms({}), clauses({}) {}

void ClauseGroup::AddClause(const std::shared_ptr<Clause> &clause_ptr) {
  // union both sets
  auto other_synonyms = clause_ptr->GetSynonyms();
  synonyms.insert(other_synonyms.begin(), other_synonyms.end());
  clauses.emplace_back(clause_ptr);
  weight += clause_ptr->GetWeight();
}

bool ClauseGroup::IsEmpty() {
  return clauses.empty();
}

bool ClauseGroup::IsConnected(const std::shared_ptr<Clause> &clause_ptr) {
  // find common element
  auto other_synonyms = clause_ptr->GetSynonyms();
  auto synonyms_it = synonyms.begin();
  auto other_it = other_synonyms.begin();
  while (synonyms_it!=synonyms.end() && other_it!=other_synonyms.end()) {
    if (*synonyms_it < *other_it) {
      ++synonyms_it;
    } else if (*other_it < *synonyms_it) {
      ++other_it;
    } else {
      return true;
    }
  }
  return false;
}

bool ClauseGroup::IsConnected(ClauseGroup &clause_group) {
  // find common element
  auto other_synonyms = clause_group.GetSynonyms();
  auto synonyms_it = synonyms.begin();
  auto other_it = other_synonyms.begin();
  while (synonyms_it!=synonyms.end() && other_it!=other_synonyms.end()) {
    if (*synonyms_it < *other_it) {
      ++synonyms_it;
    } else if (*other_it < *synonyms_it) {
      ++other_it;
    } else {
      return true;
    }
  }
  return false;
}

Table ClauseGroup::Execute() {
  std::sort(clauses.begin(), clauses.end(), ClausePtrComparator());
  Table table;
  for (auto &clause : clauses) {
    Table intermediate_table = clause->Execute();
    if (intermediate_table.IsFalseClause()) {
      return intermediate_table;
    }
    table.Merge(intermediate_table);
  }

  return table;
}

bool ClauseGroup::ExecuteBool() {
  std::sort(clauses.begin(), clauses.end(), ClausePtrComparator());
  for (auto &clause : clauses) {
    bool is_false_clause = clause->ExecuteBool();
    if (is_false_clause) {
      return true;
    }
  }
  return false;
}

size_t ClauseGroup::GetWeight() const {
  return weight;
}

size_t ClauseGroup::GetSynonymsSize() const {
  return synonyms.size();
}

std::vector<std::shared_ptr<Clause>> ClauseGroup::GetClauses() {
  return clauses;
}

std::set<std::string> ClauseGroup::GetSynonyms() {
  return synonyms;
}

void ClauseGroup::MergeClauseGroup(ClauseGroup &clause_group) {
  size_t other_weight = clause_group.GetWeight();
  auto other_synonyms = clause_group.GetSynonyms();
  auto other_clauses = clause_group.GetClauses();

  weight += other_weight;
  synonyms.insert(other_synonyms.begin(), other_synonyms.end());
  clauses.insert(clauses.end(), other_clauses.begin(), other_clauses.end());
}

}
