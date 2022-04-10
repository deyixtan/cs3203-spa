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

  for (auto &clause_group : connected_synonyms_clause_groups) {
    if (clause_group.IsConnected(select_clause_ptr)) {
      related_clause_groups.emplace_back(clause_group);
    } else {
      unrelated_clause_groups.emplace_back(clause_group);
    }
  }

  // TODO: Sort here before returning?
  return {unrelated_clause_groups, related_clause_groups};
}

}
