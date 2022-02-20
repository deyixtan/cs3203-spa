#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"
#include "sample_token.h"

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

TEST_CASE("Test invalid declaration format") {
  std::string query = "procedure p p;";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_DECLARATION_FORMAT;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid declaration synonym") {
  std::string query = "procedure 1;";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_DECLARATION_SYNONYM;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test valid declaration") {
  std::string query = "procedure p; variable v;";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> token_input = pql_lexer.Lex();
  QueryValidator query_validator = QueryValidator(token_input);
  std::vector<PqlToken> expected_tokens = {
      procedure_token,
      p_token,
      semicolon_token,
      variable_token,
      v_token,
      semicolon_token
  };

  REQUIRE(query_validator.CheckValidation() == expected_tokens);
}

TEST_CASE("Test select keyword validation") {
  std::string query = "procedure p; Selectt v";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_SELECT_KEYWORD;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid select synonym") {
  std::string query = "procedure p; Select 1";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_SELECT_SYNONYM;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid select synonym 2") {
  std::string query = "procedure p; Select _\"x\"_";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_SELECT_SYNONYM;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid select clause format") {
  std::string query = "procedure p; Select v such such";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_SELECT_CLAUSE_FORMAT;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test valid select clause") {
  std::string query = "variable v; Select v";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::vector<PqlToken> expected_tokens = {
      variable_token,
      v_token,
      semicolon_token,
      select_token,
      v_token
  };

  REQUIRE(query_validator.CheckValidation() == expected_tokens);
}

TEST_CASE("Test invalid argument for Uses") {
  std::string query = "procedure p; Select v such that Uses(1, 1)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_REL_REF_ARGUMENTS;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid argument for Modifies") {
  std::string query = "procedure p; Select v such that Modifies(1, 1)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_REL_REF_ARGUMENTS;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid rel ref format") {
  std::string query = "procedure p; Select v such that Follows((_, v)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_REL_REF_FORMAT;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test valid select with rel ref") {
  std::string query = "variable v; Select v such that Follows(1, 1)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> token_input = pql_lexer.Lex();
  QueryValidator query_validator = QueryValidator(token_input);
  std::vector<PqlToken> expected_tokens = {
      variable_token,
      v_token,
      semicolon_token,
      select_token,
      v_token,
      such_token,
      that_token,
      follow_token,
      open_parenthesis_token,
      number_value_token_1,
      comma_token,
      number_value_token_1,
      closed_parenthesis_token
  };

  REQUIRE(query_validator.CheckValidation() == expected_tokens);
}

TEST_CASE("Test invalid pattern clause format") {
  std::string query = "variable v; Select v such that Uses(1, v) pattern a aaa (_, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_PATTERN_CLAUSE_FORMAT;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid pattern keyword") {
  std::string query = "variable v; Select v such that Uses(1, v) pAttern a (_, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_PATTERN_KEYWORD;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test invalid pattern synonym") {
  std::string query = "variable v; Select v such that Uses(1, v) pattern 1 (_, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::string error = INVALID_PATTERN_SYNONYM;

  REQUIRE_THROWS_WITH(query_validator.CheckValidation(), error);
}

TEST_CASE("Test valid pattern clause with rel ref clause") {
  std::string query = "variable v; Select v such that Uses(1, v) pattern a (_,_)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::vector<PqlToken> expected_tokens = {
      variable_token,
      v_token,
      semicolon_token,
      select_token,
      v_token,
      such_token,
      that_token,
      use_token,
      open_parenthesis_token,
      number_value_token_1,
      comma_token,
      v_token,
      closed_parenthesis_token,
      pattern_token,
      a_token,
      open_parenthesis_token,
      underscore_token,
      comma_token,
      underscore_token,
      closed_parenthesis_token
  };

  REQUIRE(query_validator.CheckValidation() == expected_tokens);
}

TEST_CASE("Test valid pattern clause without rel ref clause") {
  std::string query = "variable v; Select v pattern a (_,_)";
  PqlLexer pql_lexer = PqlLexer(query);
  QueryValidator query_validator = QueryValidator(pql_lexer.Lex());
  std::vector<PqlToken> expected_tokens = {
      variable_token,
      v_token,
      semicolon_token,
      select_token,
      v_token,
      pattern_token,
      a_token,
      open_parenthesis_token,
      underscore_token,
      comma_token,
      underscore_token,
      closed_parenthesis_token
  };

  REQUIRE(query_validator.CheckValidation() == expected_tokens);
}