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
 private:
//  class ClausePtrComparator {
//    bool operator() (std::shared_ptr<Clause> clause_ptr, std::shared_ptr<Clause> other_clause_ptr) {
//      if (clause_ptr.Get)
//    }
//  };
//  int weight = 0;
  std::set<std::string> synonyms;
  std::vector<std::shared_ptr<Clause>> clauses;
};

}

#endif //CLAUSE_GROUP_H_