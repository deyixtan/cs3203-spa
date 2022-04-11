#ifndef QUERY_EVALUATOR_H_
#define QUERY_EVALUATOR_H_

#include "../pql_parser/parsed_query.h"
#include "../../pkb/pkb.h"
#include "components/query_subsystem/pql_evaluator/clauses/clause.h"
#include "components/query_subsystem/pql_evaluator/clauses/clause_factory.h"
#include "clause_util.h"
#include "components/query_subsystem/pql_evaluator/clauses/clause_group_list.h"

#include <string>
#include <unordered_set>
#include <memory>
#include <list>
#include <queue>

namespace pql {

class QueryEvaluator {
 public:
  static void Evaluate(ParsedQuery &query, const pkb::PkbPtr &pkb, std::list<std::string> &results);
 private:
  static void EvaluateOptimized(ParsedQuery &query, const pkb::PkbPtr &pkb, std::list<std::string> &results);
  static ClauseGroupList ExtractClauseGroups(ParsedQuery &query, const pkb::PkbPtr &pkb);
  static void ProjectResults(ParsedQuery &query, const pkb::PkbPtr &pkb, Table &table, std::list<std::string> &results);
  static std::queue<std::shared_ptr<pql::Clause> > ExtractClauses(ParsedQuery &query, const pkb::PkbPtr &pkb);
  static bool EvaluateNoSynonymClauseGroup(ClauseGroup &clause_group);
  static bool EvaluateUnrelatedClauseGroups(std::vector<ClauseGroup> &clause_groups);
  static Table EvaluateRelatedClauseGroups(std::vector<ClauseGroup> &clause_groups,
                                           std::shared_ptr<Clause> &select_clause_ptr);
};

}

#endif //QUERY_EVALUATOR_H_
