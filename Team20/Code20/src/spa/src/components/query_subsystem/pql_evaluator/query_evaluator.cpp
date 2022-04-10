#include "query_evaluator.h"

namespace pql {

void QueryEvaluator::Evaluate(ParsedQuery &query, const PkbPtr &pkb, std::list<std::string> &results) {
  EvaluateOptimized(query, pkb, results);
  pkb->GetAffectsStore()->ClearAffectsSession();
  pkb->GetNextStore()->ClearNextStarCache();
}

void QueryEvaluator::EvaluateUnoptimized(ParsedQuery &query, const PkbPtr &pkb, std::list<std::string> &results) {
  pql::Table table;
  auto clauses = ExtractClauses(query, pkb);
  // extract clause -> optimizer -> sort clauses?
  // or insert a new sorter in here after extracting?
  while (!clauses.empty()) {
    auto clause = std::move(clauses.front());
    auto intermediate_table = clause->Execute();
    table.Merge(intermediate_table);
    clauses.pop();
  }

  ProjectResults(query, pkb, table, results);
}

bool QueryEvaluator::EvaluateNoSynonymClauseGroup(ClauseGroup &clause_group) {
  if (!clause_group.IsEmpty()) {
    bool is_false_clause_encountered = clause_group.ExecuteBool();
    return is_false_clause_encountered;
  }
  return false;
}

bool QueryEvaluator::EvaluateUnrelatedClauseGroups(std::vector<ClauseGroup> &clause_groups) {
  for (auto &clause_group : clause_groups) {
    bool is_false_clause = clause_group.Execute().IsFalseClause();
    if (is_false_clause) {
      return true;
    }
  }
  return false;
}

Table QueryEvaluator::EvaluateRelatedClauseGroups(std::vector<ClauseGroup> &clause_groups, std::shared_ptr<Clause> &select_clause_ptr) {
  Table table;
  for (auto &clause_group : clause_groups) {
    Table intermediate_table = clause_group.Execute();
    if (intermediate_table.IsFalseClause()) {
      table = std::move(intermediate_table);
      break;
    }
    intermediate_table.Filter(select_clause_ptr->GetSynonyms());
    table.Merge(intermediate_table);
  }
  return table;
}

void QueryEvaluator::EvaluateOptimized(ParsedQuery &query, const PkbPtr &pkb, std::list<std::string> &results) {
  auto clause_group_list = ExtractClauseGroups(query, pkb);
  auto no_synonyms_clause_group = clause_group_list.GetNoSynonymsClauseGroup();
  auto select_clause_ptr = ClauseFactory::Create(query.GetResultClause(), query.GetDeclaration().GetDeclarations(), pkb);
  auto clause_groups_pair = clause_group_list.SeparateSynonymsClauseGroups(select_clause_ptr);
  auto unrelated_clause_groups = clause_groups_pair.first;
  auto related_clause_groups = clause_groups_pair.second;

  bool is_false_clause_encountered =
      EvaluateNoSynonymClauseGroup(no_synonyms_clause_group) || EvaluateUnrelatedClauseGroups(unrelated_clause_groups);
  Table table;
  if (!is_false_clause_encountered) {
    table = std::move(EvaluateRelatedClauseGroups(related_clause_groups, select_clause_ptr));
  } else {
    table.ToggleFalseClause();
  }

  Table select_table = select_clause_ptr->Execute();
  table.Merge(select_table);
  ProjectResults(query, pkb, table, results);
}

ClauseGroupList QueryEvaluator::ExtractClauseGroups(ParsedQuery &query, const PkbPtr &pkb) {
  ClauseGroupList clause_group_list;

  auto declarations = query.GetDeclaration().GetDeclarations();
  for (const auto &relationship : query.GetRelationships()) {
    auto clause = ClauseFactory::Create(relationship, declarations, pkb);
    if (clause) {
      clause_group_list.AddClause(clause);
    }
  }
  for (const auto &pattern : query.GetPatterns()) {
    auto clause = ClauseFactory::Create(pattern, declarations, pkb);
    if (clause) {
      clause_group_list.AddClause(clause);
    }
  }
  for (const auto &with : query.GetWithClause()) {
    auto clause = ClauseFactory::Create(with, declarations, pkb);
    if (clause) {
      clause_group_list.AddClause(clause);
    }
  }

  return clause_group_list;
}

void QueryEvaluator::ProjectResults(ParsedQuery &query,
                                    const PkbPtr &pkb,
                                    Table &table,
                                    std::list<std::string> &results) {
  if (table.IsBooleanResult()) {
    if (table.IsFalseClause()) {
      results.emplace_back("FALSE");
    } else {
      results.emplace_back("TRUE");
    }
  } else if (table.IsAttributeResult()) {
    ResultClause result_clause = query.GetResultClause();
    std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();
    std::pair<std::pair<DesignEntityType, std::string>, AttriName>
        attribute = Utils::ParseAttributeRef(result_clause.GetValues().front(), declarations);
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
  } else if (table.IsSynonymResult()) {
    auto projected_results = table.GetResult(query.GetResultClause().GetValues()[0].value);
    for (auto result : projected_results) {
      results.emplace_back(result);
    }
  } else if (table.IsTupleResult()) {
    auto projected_results =
        table.GetTupleResult(query.GetResultClause().GetValues(), query.GetDeclaration().GetDeclarations(), pkb);
    for (auto result : projected_results) {
      results.emplace_back(result);
    }
  }
}

std::queue<std::shared_ptr<pql::Clause> > QueryEvaluator::ExtractClauses(ParsedQuery &query, const PkbPtr &pkb) {
  std::queue<std::shared_ptr<pql::Clause> > clauses;
  for (const auto &relationship : query.GetRelationships()) {
    auto clause = pql::ClauseFactory::Create(relationship, query.GetDeclaration().GetDeclarations(), pkb);
    if (clause) {
      clauses.push(std::move(clause));
    }
  }

  for (const auto &pattern : query.GetPatterns()) {
    auto clause = pql::ClauseFactory::Create(pattern, query.GetDeclaration().GetDeclarations(), pkb);
    if (clause) {
      clauses.push(std::move(clause));
    }
  }

  for (const auto &with : query.GetWithClause()) {
    auto clause = pql::ClauseFactory::Create(with, query.GetDeclaration().GetDeclarations(), pkb);
    if (clause) {
      clauses.push(std::move(clause));
    }
  }

  clauses.push(pql::ClauseFactory::Create(query.GetResultClause(), query.GetDeclaration().GetDeclarations(), pkb));

  // return clause group
  return clauses;
}

}