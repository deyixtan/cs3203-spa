#include "query_evaluator.h"

namespace pql_evaluator {

void QueryEvaluator::Evaluate(ParsedQuery &query, std::list<std::string> &results) {
  pql::Table table;
  auto clauses = ExtractClauses(query);
  // extract clause -> optimizer -> sort clauses?
  // or insert a new sorter in here after extracting?
  while (!clauses.empty()) {
    auto clause = std::move(clauses.front());
    auto intermediate_table = clause->Execute();
    table.Merge(intermediate_table);
    clauses.pop();
  }

  if (table.IsBooleanResult()) {
    if (table.HasEncounteredFalseClause()) {
      results.emplace_back("FALSE");
    } else {
      results.emplace_back("TRUE");
    }
  } else if (table.IsAttributeResult()) {
    ResultClause result_clause = query.GetResultClause();
    std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();
    std::pair<std::pair<DesignEntityType, std::string>, AtrriName> attribute = Utils::ParseAttributeRef(result_clause.GetValues().front(), declarations);
    auto projected_results = table.GetResult(attribute.first.second);
    if (Utils::IsConversionNeeded(attribute.first.first, attribute.second)) {
      std::unordered_set<std::string> temp_set;
      for (auto s : projected_results) {
        temp_set.insert(pkb->GetNameByStmt(clause_util::GetStmtType(attribute.first.first), s));
      }
      for (auto result : temp_set) {
        results.emplace_back(result);
      }
    } else {
      for (auto result : projected_results) {
        results.emplace_back(result);
      }
    }
  } else {
    auto projected_results = table.GetResult(query.GetResultClause().GetValues()[0].value);
    for (auto result : projected_results) {
      results.emplace_back(result);
    }
  }

}

std::queue<std::unique_ptr<pql::Clause> > QueryEvaluator::ExtractClauses(ParsedQuery &query) {
  std::queue<std::unique_ptr<pql::Clause> > clauses;
  for (const auto& relationship : query.GetRelationships()) {
    auto clause = pql::ClauseFactory::Create(relationship, query.GetDeclaration(), pkb);
    if (clause) {
      clauses.push(std::move(clause));
    }
  }

  for (const auto& pattern : query.GetPatterns()) {
    auto clause = pql::ClauseFactory::Create(pattern, query.GetDeclaration(), pkb);
    if (clause) {
      clauses.push(std::move(clause));
    }
  }

  for (const auto& with : query.GetWithClause()) {
    auto clause = pql::ClauseFactory::Create(with, query.GetDeclaration(), pkb);
    if (clause) {
      clauses.push(std::move(clause));
    }
  }

  clauses.push(pql::ClauseFactory::Create(query.GetResultClause(), query.GetDeclaration(), pkb));

  return clauses;
}


}