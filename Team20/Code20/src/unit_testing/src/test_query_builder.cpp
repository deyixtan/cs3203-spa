#include "sample_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.cpp"
#include "components/query_subsystem/pql_parser/parsed_query.h"

#include "catch.hpp"

TEST_CASE("Test query parser with uses") {
  std::vector<PqlToken> test_token_vect;

  // stmt
  test_token_vect.push_back(stmt_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(semicolon_token);

  // variable
  test_token_vect.push_back(variable_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(such_token);
  test_token_vect.push_back(that_token);
  test_token_vect.push_back(use_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(closed_parenthesis_token);

  ParsedQuery pq  = ParsedQuery().BuildParsedQuery(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::optional<Relationship> rship = pq.GetRelationship();
  std::optional<Pattern> patt = pq.GetPattern();
  std::vector<Declaration> decl = pq.GetDeclaration();

  REQUIRE(rship->GetRelRef().value == "Uses");
  REQUIRE(rship->GetFirst().value == "s");
  REQUIRE(rship->GetSecond().value == "v");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[1].GetSynonym().value == "v");
}

TEST_CASE("Test query parser with uses and pattern") {
  std::vector<PqlToken> test_token_vect;

  // stmt
  test_token_vect.push_back(stmt_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(semicolon_token);

  // variable
  test_token_vect.push_back(variable_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(semicolon_token);

  // assign
  test_token_vect.push_back(assign_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(such_token);
  test_token_vect.push_back(that_token);
  test_token_vect.push_back(use_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(x_string_token);
  test_token_vect.push_back(closed_parenthesis_token);
  test_token_vect.push_back(pattern_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  ParsedQuery pq = ParsedQuery().BuildParsedQuery(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::optional<Relationship> rship = pq.GetRelationship();
  std::optional<Pattern> patt = pq.GetPattern();
  std::vector<Declaration> decl = pq.GetDeclaration();

  REQUIRE(rship->GetRelRef().value == "Uses");
  REQUIRE(rship->GetFirst().value == "s");
  REQUIRE(rship->GetSecond().value == "\"x\"");
  REQUIRE(synonym.value == "s");
  REQUIRE(patt->GetSynAssign().value == "a");
  REQUIRE(patt->GetFirst().value == "v");
  REQUIRE(patt->GetSecond().value == "_");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[1].GetSynonym().value == "v");
  REQUIRE(decl[2].GetDesignEntity().value == "assign");
  REQUIRE(decl[2].GetSynonym().value == "a");
}

TEST_CASE("Test query parser with multiple variables") {
  std::vector<PqlToken> test_token_vect;

  // stmt
  test_token_vect.push_back(stmt_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(semicolon_token);

  // variable
  test_token_vect.push_back(variable_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(v1_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(such_token);
  test_token_vect.push_back(that_token);
  test_token_vect.push_back(parent_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(number_value_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(closed_parenthesis_token);

  //To Do: Fix this after refractoring
  ParsedQuery pq = ParsedQuery().BuildParsedQuery(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::optional<Relationship> rship = pq.GetRelationship();
  std::optional<Pattern> patt = pq.GetPattern();
  std::vector<Declaration> decl = pq.GetDeclaration();

  REQUIRE(rship->GetRelRef().value == "Parent");
  REQUIRE(rship->GetFirst().value == "1");
  REQUIRE(rship->GetSecond().value == "s");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetSynonym().value == "v");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[2].GetDesignEntity().value == "variable");
  REQUIRE(decl[2].GetSynonym().value == "v1");
  REQUIRE(decl[3].GetDesignEntity().value == "variable");
  REQUIRE(decl[3].GetSynonym().value == "a");
}

TEST_CASE("Test select without such that") {
  std::vector<PqlToken> test_token_vect;

  // stmt
  test_token_vect.push_back(procedure_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(s_token);

  //BuildParsedQuery(test_token_vect);
  ParsedQuery pq = ParsedQuery().BuildParsedQuery(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::optional<Relationship> rship = pq.GetRelationship();
  std::optional<Pattern> patt = pq.GetPattern();
  std::vector<Declaration> decl = pq.GetDeclaration();

  REQUIRE(synonym.value == "s");
}
