#include "query_evaluator.h"

namespace pql_evaluator {

void QueryEvaluator::Evaluate(ParsedQuery& query, std::list<std::string>& results) {
  result.clear();
  EvaluateSelect(query);
  for (auto i = result.begin(); i != result.end(); ++i) {
    results.emplace_back(*i);
  }
}

void QueryEvaluator::EvaluateSelect(ParsedQuery& query) {
  const PqlToken select_synonym = query.GetSynonym();
  // TODO::
  const auto declarations = query.GetDeclaration();
  PqlToken token; // uninit

  // pass thru validation
  for (auto declaration : declarations) {
    if (declaration.GetSynonym().value == select_synonym.value) {
      token = declaration.GetDesignEntity();
    }
  }
  std::unordered_set<std::string> add_result;
  switch (token.type) {
    case PqlTokenType::PROCEDURE:
      add_result = pkb->GetStmt(StmtType::PROC);
      result.insert(add_result.begin(), add_result.end());
      break;
    case PqlTokenType::VARIABLE:
      add_result = pkb->GetStmt(StmtType::VARS);
      result.insert(add_result.begin(), add_result.end());
      break;
    case PqlTokenType::CONSTANT:
      add_result = pkb->GetStmt(StmtType::CONSTS);
      result.insert(add_result.begin(), add_result.end());
      break;
    case PqlTokenType::STMT:
      auto temp = pkb->GetStmt(StmtType::STMT);
      for (auto i = temp.begin(); i != temp.end(); ++i) {
        add_result.insert(*i);
      }
      result.insert(add_result.begin(), add_result.end());
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