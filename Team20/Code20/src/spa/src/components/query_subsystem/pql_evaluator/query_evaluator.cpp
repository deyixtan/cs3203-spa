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
  const auto declarations = query.GetDeclaration();
  PqlToken token; // uninit

  // pass thru validation
  // TODO: refactor ParsedQuery to ValidQuery object
  for (auto declaration : declarations) {
    if (declaration.GetSynonym().value == select_synonym.value) {
      token = declaration.GetDesignEntity();
    }
  }

  // if no such that and no pattern
  if (query.GetRelationships().empty() && query.GetPatterns().empty()) {
    std::unordered_set<std::string> add_result;
    switch (token.type) {
      case PqlTokenType::STMT: {
        add_result = pkb->GetStmt(StmtType::STMT);
        result.insert(add_result.begin(), add_result.end());
        break;
      }
      case PqlTokenType::READ: {
        add_result = pkb->GetStmt(StmtType::READ);
        result.insert(add_result.begin(), add_result.end());
      }
      case PqlTokenType::PRINT: {
        add_result = pkb->GetStmt(StmtType::PRINT);
        result.insert(add_result.begin(), add_result.end());
      }
      case PqlTokenType::WHILE: {
        add_result = pkb->GetStmt(StmtType::WHILE);
        result.insert(add_result.begin(), add_result.end());
      }
      case PqlTokenType::IF: {
        add_result = pkb->GetStmt(StmtType::IF);
        result.insert(add_result.begin(), add_result.end());
      }
      case PqlTokenType::VARIABLE: {
        add_result = pkb->GetStmt(StmtType::VARS);
        result.insert(add_result.begin(), add_result.end());
        break;
      }
      case PqlTokenType::CONSTANT: {
        add_result = pkb->GetStmt(StmtType::CONSTS);
        result.insert(add_result.begin(), add_result.end());
        break;
      }
      case PqlTokenType::PROCEDURE: {
        add_result = pkb->GetStmt(StmtType::PROC);
        result.insert(add_result.begin(), add_result.end());
        break;
      }
      default: {
        // TODO:throw exception
      }
    }
  }

}

}
