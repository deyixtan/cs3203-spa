#include "query_controller.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/pql_grammar_validator/query_validator.h"

QueryController::QueryController(PKB *pkb): validator_{new pql_validator::ParsedQueryValidator()}, evaluator_{new pql_evaluator::QueryEvaluator(pkb)} {}

void QueryController::ProcessQuery(std::string query, std::list<std::string> &results) {
  try {
    PqlLexer lexer{query};
    std::vector<PqlToken> tokens = lexer.Lex();
    QueryValidator query_validator = QueryValidator(tokens);
    std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
    ParsedQueryBuilder pqb(validated_tokens);
    ParsedQuery parsed_query = pqb.Build();
    if (validator_->ValidateQuery(parsed_query)) {
      evaluator_->Evaluate(parsed_query, results);
      evaluator_->WipeCache();
    } else if (parsed_query.GetResultClause().GetType() == ResultClauseType::BOOLEAN){
      results.push_back("FALSE");
    }
  } catch (std::exception &e1) {
    std::cout << e1.what() << std::endl;
  } catch (std::string &e2) {
    std::cout << e2 << std::endl;
  } catch (char* &e3) {
    std::cout << e3 << std::endl;
  }
}
