#include "query_controller.h"

QueryController::QueryController(PKB *pkb): validator_{new pql_validator::ParsedQueryValidator()}, evaluator_{new pql_evaluator::QueryEvaluator(pkb)} {}

void QueryController::ProcessQuery(std::string query, std::list<std::string> &results) {
  PqlLexer lexer {query};
  ParsedQuery pqb {};
  std::vector<PqlToken> tokens = lexer.Lex();
  QueryValidator query_validator = QueryValidator(tokens);
  query_validator.CheckValidation();
  ParsedQuery parsed_query = pqb.BuildParsedQuery(tokens);
  validator_->ValidateQuery(parsed_query);
  evaluator_->Evaluate(parsed_query, results);
}
