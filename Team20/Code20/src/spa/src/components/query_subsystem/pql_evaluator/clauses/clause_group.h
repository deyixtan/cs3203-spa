#ifndef CLAUSE_GROUP_H_
#define CLAUSE_GROUP_H_

#include "components/query_subsystem/pql_evaluator/clauses/clause.h"
#include "components/query_subsystem/pql_evaluator/clause_util.h"
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
  bool IsConnected(ClauseGroup &clause_group);
  void MergeClauseGroup(ClauseGroup &clause_group);
  Table Execute();
  bool ExecuteBool();
  [[nodiscard]] size_t GetWeight() const;
  [[nodiscard]] size_t GetSynonymsSize() const;
  std::vector<std::shared_ptr<Clause>> GetClauses();
  std::set<std::string> GetSynonyms();
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
  size_t weight = 0;
  std::vector<std::shared_ptr<Clause>> clauses;
  std::set<std::string> synonyms;
};

}

#endif //CLAUSE_GROUP_H_