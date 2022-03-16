#include "catch.hpp"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_validator/parsed_query_validator.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"

TEST_CASE("Test duplicated declarations") {
  std::string query = "stmt s; variable v, s; assign a;\n Select s1";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  REQUIRE_THROWS_WITH(pqb.Build(), "ERROR: Duplicate synonym! \n");
}

TEST_CASE("Test query missing declaration in result clause") {
  std::string query = "stmt s; variable v; assign a;\n Select s1";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query missing declaration in relationship clause") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Uses (s1, v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query missing declaration in pattern clause") {
  std::string query = "stmt s; variable v; assign a;\n Select s pattern a (v1, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query no missing declaration in pattern clause synonym") {
  std::string query = "stmt s; variable v; assign a;\n Select s pattern a1 (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query no missing declaration in pattern clause") {
  std::string query = "stmt s; variable v; assign a;\n Select s pattern a (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query missing declaration in with clause 1") {
  std::string query = "stmt s; variable v; assign a;\n Select s with s1.stmt# = 2";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query missing declaration in with clause 2") {
  std::string query = "stmt s; variable v; assign a;\n Select s with 1 = s1.stmt#";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query missing declaration in with clause 3") {
  std::string query = "stmt s; variable v; assign a;\n Select s with s.stmt# = s1.stmt#";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong with clause") {
  std::string query = "stmt s; variable v; assign a;\n Select s with s.stmt# = 1 and s.stmt# = v.varName";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct with clause") {
  std::string query = "stmt s; constant c; assign a;\n Select s with s.stmt# = c.value";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong attribute") {
  std::string query = "stmt s; variable v; assign a;\n Select s.varName";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong attribute in tuple") {
  std::string query = "stmt s; variable v; assign a;\n Select <s.varName, s>";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct attribute in tuple") {
  std::string query = "stmt s; variable v; assign a;\n Select <v.varName, s>";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong attribute in with clause") {
  std::string query = "stmt s; variable v; assign a;\n Select <v.varName, s> with s.value = 3";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct multi with clause") {
//  std::string query = "stmt s, s1; variable v; constant c; assign a;\n Select s with s.stmt# = c.value and s1.stmt# = 2 and \"varName\" = v.varName";
//  PqlLexer pql_lexer = PqlLexer(query);
//  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
//  QueryValidator qv = QueryValidator(test_token_vect);
//  test_token_vect = qv.CheckValidation();
//  ParsedQueryBuilder pqb(test_token_vect);
//  ParsedQuery pq = pqb.Build();
//  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
//  bool is_query_valid = pqv.ValidateQuery(pq);
//
//  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong with clause 1") {
//  std::string query = "stmt s, s1; variable v; constant c; assign a;\n Select s with 2 = v.varName;
//  PqlLexer pql_lexer = PqlLexer(query);
//  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
//  QueryValidator qv = QueryValidator(test_token_vect);
//  test_token_vect = qv.CheckValidation();
//  ParsedQueryBuilder pqb(test_token_vect);
//  ParsedQuery pq = pqb.Build();
//  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
//  bool is_query_valid = pqv.ValidateQuery(pq);
//
//  REQUIRE(is_query_valid == wrong);
}

TEST_CASE("Test query wrong with clause 2") {
//  std::string query = "stmt s, s1; variable v; constant c; assign a;\n Select s with 2 = v.varName and s.stmt# = 1;
//  PqlLexer pql_lexer = PqlLexer(query);
//  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
//  QueryValidator qv = QueryValidator(test_token_vect);
//  test_token_vect = qv.CheckValidation();
//  ParsedQueryBuilder pqb(test_token_vect);
//  ParsedQuery pq = pqb.Build();
//  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
//  bool is_query_valid = pqv.ValidateQuery(pq);
//
//  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong with clause 3") {
//  std::string query = "stmt s, s1; variable v; constant c; assign a;\n Select s with s.stmt# = 1 and 2 = v.varName;
//  PqlLexer pql_lexer = PqlLexer(query);
//  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
//  QueryValidator qv = QueryValidator(test_token_vect);
//  test_token_vect = qv.CheckValidation();
//  ParsedQueryBuilder pqb(test_token_vect);
//  ParsedQuery pq = pqb.Build();
//  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
//  bool is_query_valid = pqv.ValidateQuery(pq);
//
//  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong Uses clause argument") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Uses (s, a)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong multi Uses clause argument") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Uses(s, v) and Uses (s, a)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct Uses clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Uses (s, v) and Uses (s1, v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong modifies clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Modifies (s, s1)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong multi modifies clause argument 1") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Modifies (s, s1) and Modifies(s, v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong multi modifies clause argument 2") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Next(s, s) and Modifies (s, s1)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct modifies clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Modifies (s, v) and Uses (s1, v) with s.stmt# = 1";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong parent clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Parent (s, v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong next clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Next (s, v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct next and affects clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Next (s, s) and Next* (s, s) and Affects (s, s)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query correct assign pattern clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s such that Next (s, s) and Next* (s, s) pattern a (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong assign pattern clause argument") {
  std::string query = "stmt s, s1; variable v; assign a;\n Select s pattern a (s, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct while pattern clause argument") {
  std::string query = "stmt s, s1; variable v; assign a; while w;\n Select s pattern w (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong while pattern clause argument") {
  std::string query = "stmt s, s1; variable v; assign a; while w;\n Select s pattern w (s, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct while and if pattern clause argument") {
  std::string query = "stmt s, s1; variable v; assign a; while w; if ifs; \n Select s pattern w (v, _) and ifs (v,_,_)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

TEST_CASE("Test query wrong if pattern clause argument") {
  std::string query = "stmt s, s1; variable v; assign a; while w; if ifs; \n Select s pattern w (v, _) and ifs (s,_,_)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query wrong multi clause") {
  std::string query = "stmt s, s1; variable v; assign a; while w; if ifs; \n Select s such that Uses (s, v) pattern w (v, _) and ifs (s,_,_)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == false);
}

TEST_CASE("Test query correct multi clause") {
  std::string query = "stmt s, s1; variable v; assign a; while w; if ifs; \n Select s such that Uses (s, v) pattern w (v, _) and ifs (v,_,_) with s.stmt# = 2";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  pql_validator::ParsedQueryValidator pqv = pql_validator::ParsedQueryValidator();
  bool is_query_valid = pqv.ValidateQuery(pq);

  REQUIRE(is_query_valid == true);
}

