#ifndef CLAUSE_GROUP_LIST_H_
#define CLAUSE_GROUP_LIST_H_

#include "clause.h"
#include "clause_group.h"
#include "vector"

namespace pql {

class ClauseGroupList {
 public:
  ClauseGroupList();
  void AddClause(const std::shared_ptr<Clause> &clause_ptr);
  ClauseGroup GetNoSynonymsClauseGroup();
  std::pair<std::vector<ClauseGroup>, std::vector<ClauseGroup>> SeparateSynonymsClauseGroups(const std::shared_ptr<Clause>& select_clause_ptr);
 private:
  ClauseGroup no_synonyms_clause_group;
  std::vector<ClauseGroup> connected_synonyms_clause_groups;
};

}

#endif //CLAUSE_GROUP_LIST_H_
