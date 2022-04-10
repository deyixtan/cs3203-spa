#include "query_controller.h"

#include <utility>
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/pql_parser.h"

namespace pql {

void QueryController::ProcessQuery(std::string query, const PkbPtr &pkb, std::list<std::string> &results) {
  try {
    PqlLexer lexer{std::move(query)};
    std::vector<PqlToken> tokens = lexer.Lex();
    PqlParser pql_parser = PqlParser(tokens);
    ParsedQuery parsed_query = pql_parser.ParseQuery();
    if (ParsedQueryValidator::IsQuerySemanticallyValid(parsed_query)) {
      QueryEvaluator::Evaluate(parsed_query, pkb, results);
    } else if (parsed_query.GetResultClause().GetType()==ResultClauseType::BOOLEAN) {
      results.emplace_back("FALSE");
    }
  } catch (std::exception &e1) {
    std::cout << e1.what() << std::endl;
  } catch (std::string &e2) {
    std::cout << e2 << std::endl;
  } catch (char *&e3) {
    std::cout << e3 << std::endl;
  }
}

}
