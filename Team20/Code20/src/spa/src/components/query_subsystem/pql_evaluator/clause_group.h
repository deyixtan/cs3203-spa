#ifndef CLAUSE_GROUP_H_
#define CLAUSE_GROUP_H_

#include "clause.h"
#include "clause_util.h"
#include <set>
#include <string>
#include <queue>

namespace pql {

class ClauseGroup {
 public:
  ClauseGroup();
  void AddClause(const std::shared_ptr<Clause> &clause_ptr);
  bool IsEmpty();
  bool IsConnected(const std::shared_ptr<Clause> &clause_ptr);
  Table Execute();
  bool ExecuteBool();
  size_t GetWeight() const;
  size_t GetSynonymsSize() const;
 private:
  class ClausePtrComparator {
   public:
    bool operator()(const std::shared_ptr<Clause> &clause_ptr, const std::shared_ptr<Clause> &other_clause_ptr) {
      // Compare by number of synonyms in the clause first -> prioritize less synonyms
      if (clause_ptr->GetSynonymsSize() < other_clause_ptr->GetSynonymsSize()) return true;
      if (clause_ptr->GetSynonymsSize() > other_clause_ptr->GetSynonymsSize()) return false;
      // Prioritize clauses with smaller results based on static weight approximation;
      return clause_ptr->GetWeight() < other_clause_ptr->GetWeight();
    }
  };
  int weight = 0;
  std::set<std::string> synonyms;
  std::vector<std::shared_ptr<Clause>> clauses;
};

}

#endif //CLAUSE_GROUP_H_