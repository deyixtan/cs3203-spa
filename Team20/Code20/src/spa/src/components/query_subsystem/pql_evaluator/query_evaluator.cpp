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
  std::unordered_set<int> temp;
  switch (token.type) {
    case PqlTokenType::PROCEDURE: {
      add_result = pkb->get_stmt_by_name(StmtType::PROC);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::VARIABLE: {
      add_result = pkb->get_stmt_by_name(StmtType::VARS);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::CONSTANT: {
      add_result = pkb->get_stmt_by_name(StmtType::CONSTS);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::STMT: {
      std::unordered_set<int> temp = pkb->get_stmt_by_num(StmtType::STMT);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::WHILE: {
      temp = pkb->get_stmt_by_num(StmtType::WHILE);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::READ: {
      temp = pkb->get_stmt_by_num(StmtType::READ);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::PRINT: {
      temp = pkb->get_stmt_by_num(StmtType::PRINT);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::CALL: {
      temp = pkb->get_stmt_by_num(StmtType::CALL);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::IF: {
      temp = pkb->get_stmt_by_num(StmtType::IF);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::ASSIGN: {
      temp = pkb->get_stmt_by_num(StmtType::ASSIGN);
      for (auto i = temp.begin(); i!=temp.end(); ++i) {
        add_result.insert(std::to_string(*i));
      }
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    default: {
      // TODO:throw exception
    }
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

  return new_result;
}

}