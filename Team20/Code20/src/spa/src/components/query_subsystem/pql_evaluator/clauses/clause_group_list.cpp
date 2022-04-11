#include "clause_group_list.h"

namespace pql {

ClauseGroupList::ClauseGroupList() : no_synonyms_clause_group({}), connected_synonyms_clause_groups({}) {}

void ClauseGroupList::AddClause(const std::shared_ptr<Clause> &clause_ptr) {
  if (clause_ptr->GetSynonymsSize()==0) {
    no_synonyms_clause_group.AddClause(clause_ptr);
  } else {
    for (auto &clause_group : connected_synonyms_clause_groups) {
      if (clause_group.IsConnected(clause_ptr)) {
        clause_group.AddClause(clause_ptr);
        return;
      }
    }

    // Create new group if not connected to any existing one
    ClauseGroup new_group;
    new_group.AddClause(clause_ptr);
    connected_synonyms_clause_groups.emplace_back(new_group);
  }
}

ClauseGroup ClauseGroupList::GetNoSynonymsClauseGroup() {
  return no_synonyms_clause_group;
}

std::pair<std::vector<ClauseGroup>,
          std::vector<ClauseGroup>> ClauseGroupList::SeparateSynonymsClauseGroups(const std::shared_ptr<Clause> &select_clause_ptr) {
  std::vector<ClauseGroup> unrelated_clause_groups;
  std::vector<ClauseGroup> related_clause_groups;

  for (auto i = connected_synonyms_clause_groups.size() - 1; i > 0; --i) {
    for (auto j = i - 1; j >= 0; --j) {
      if (connected_synonyms_clause_groups.at(j).IsConnected(connected_synonyms_clause_groups.at(i))) {
        connected_synonyms_clause_groups.at(j).MergeClauseGroup(connected_synonyms_clause_groups.at(i));
        connected_synonyms_clause_groups.erase(connected_synonyms_clause_groups.begin() + i);
        break;
      }
    }
  }

  for (auto &clause_group : connected_synonyms_clause_groups) {
    if (clause_group.IsConnected(select_clause_ptr)) {
      related_clause_groups.emplace_back(clause_group);
    } else {
      unrelated_clause_groups.emplace_back(clause_group);
    }
  }

  std::sort(unrelated_clause_groups.begin(), unrelated_clause_groups.end(), ClauseGroupComparator());
  std::sort(related_clause_groups.begin(), related_clause_groups.begin(), ClauseGroupComparator());
  return {unrelated_clause_groups, related_clause_groups};
}

}
