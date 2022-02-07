#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"
#include "sample_token.h"
#include "components/source_subsystem/node.h"


#include "catch.hpp"

TEST_CASE("Test missing token declaration validation") {
  std::string query = "procedure p";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = MISSING_TOKEN_IN_DECLARATION;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test wrong design entity declaration validation") {
  std::string query = "pRoCedUre p;";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_DESIGN_ENTITY_IN_DECLARATION;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test select keyword validation") {
  std::string query = "procedure p; Selectt v";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_SELECT_KEYWORD;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}
