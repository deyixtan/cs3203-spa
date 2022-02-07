#include "query_evaluator.h"

namespace pql_evaluator {

std::unordered_set<std::string> QueryEvaluator::Evaluate(ParsedQuery& query) {
  result.clear();
  EvaluateSelect(query);
}

void QueryEvaluator::EvaluateSelect(ParsedQuery& query) {
  const PqlToken select_synonym = query.GetSynonym();
  std::unordered_set<std::string> add_result;
  switch (select_synonym.type) {
    case PqlTokenType::PROCEDURE:
      add_result = pkb->get_stmt_by_name(StmtType::PROC);
      result.insert(add_result.begin(), add_result.end());
      break;
    case PqlTokenType::VARIABLE:
      add_result = pkb->get_stmt_by_name(StmtType::VARS);
      result.insert(add_result.begin(), add_result.end());
      break;
    case PqlTokenType::CONSTANT:
      add_result = pkb->get_stmt_by_name(StmtType::CONSTS);
      result.insert(add_result.begin(), add_result.end());
      break;
    default:
      break;
      //fall through
  }
  // TODO:
  //  1. call pkb to get based on type of select synonym
  //  2. populate the result set
}

// utility to intersect two result sets
std::unordered_set<std::string> Intersect(std::unordered_set<std::string> s1, std::unordered_set<std::string> s2) {
  // assume s1 > s2 equivalent to s2 <= s1
  if (s2.size() > s1.size()) {
    return Intersect(s2, s1);
  }

  std:: unordered_set<std::string> new_result;
  for (auto i = s1.begin(); i != s1.end(); i++) {
    if (s2.find(*i) != s2.end()) {
      new_result.insert(*i);
    }
  }
}

}