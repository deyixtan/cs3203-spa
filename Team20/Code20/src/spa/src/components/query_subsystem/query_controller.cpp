#include "query_controller.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_validator.h"

QueryController::QueryController(PKB *pkb): validator_{new pql_validator::ParsedQueryValidator()}, evaluator_{new pql_evaluator::QueryEvaluator(pkb)} {}

void QueryController::ProcessQuery(std::string query, std::list<std::string> &results) {
  PqlLexer lexer {query};
  ParsedQueryBuilder pqb;
  std::vector<PqlToken> tokens = lexer.Lex();
  QueryValidator query_validator = QueryValidator(tokens);
  std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
  ParsedQuery parsed_query = pqb.Build(validated_tokens);
  if (validator_->ValidateQuery(parsed_query)) {
    evaluator_->Evaluate(parsed_query, results);
  }
}
