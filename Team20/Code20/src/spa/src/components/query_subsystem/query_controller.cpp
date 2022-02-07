#include "query_controller.h"

QueryController::QueryController(PKB *pkb): validator_{new pql_validator::ParsedQueryValidator()}, evaluator_{new pql_evaluator::QueryEvaluator(pkb)} {}

void QueryController::ProcessQuery(std::string query, std::list<std::string> &results) {
  PqlLexer lexer {query};
  ParsedQuery parsed_query {};
  std::vector<PqlToken> tokens = lexer.Lex();
  QueryValidator query_validator = QueryValidator(tokens);
  query_validator.CheckValidation();
  parsed_query.BuildParsedQuery(tokens);
  validator_->ValidateQuery(parsed_query);
  std::unordered_set<std::string> res = evaluator_->Evaluate(parsed_query);
  for (auto i = res.begin(); i != res.end(); ++i) {
    results.emplace_back(*i);
  }
}
