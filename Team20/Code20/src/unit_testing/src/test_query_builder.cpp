#include "sample_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.cpp"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"

#include "catch.hpp"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"

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

  //BuildParsedQuery(test_token_vect);
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
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

  //BuildParsedQuery(test_token_vect);
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  Pattern patt = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "\"x\"");
  REQUIRE(synonym.value == "s");
  REQUIRE(patt.GetSynAssign().value == "a");
  REQUIRE(patt.GetFirst().value == "v");
  REQUIRE(patt.GetSecond().value == "_");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
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
  test_token_vect.push_back(number_value_token_1);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(closed_parenthesis_token);

  //BuildParsedQuery(test_token_vect);
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration();

  REQUIRE(rship.GetRelRef().value == "Parent");
  REQUIRE(rship.GetFirst().value == "1");
  REQUIRE(rship.GetSecond().value == "s");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("v1")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::VARIABLE);
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

  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();\
  const auto decl = pq.GetDeclaration();

  REQUIRE(synonym.value == "s");
}

TEST_CASE("Test query parser with pattern without such that") {
  std::vector<PqlToken> test_token_vect;

  // assign
  test_token_vect.push_back(assign_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(semicolon_token);

  // variable
  test_token_vect.push_back(variable_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(pattern_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::optional<Pattern> patt = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration();

  REQUIRE(synonym.value == "a");
  REQUIRE(patt->GetSynAssign().value == "a");
  REQUIRE(patt->GetFirst().value == "v");
  REQUIRE(patt->GetSecond().value == "_");
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
}

TEST_CASE("Test query parser with repeated synonym") {
  std::vector<PqlToken> test_token_vect;

  // assign
  test_token_vect.push_back(assign_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(semicolon_token);

  // variable
  test_token_vect.push_back(variable_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(pattern_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  ParsedQueryBuilder pqb;
  std::string error = DUPLICATE_DECLARATION_SYNONYM;

  REQUIRE_THROWS_WITH(pqb.Build(test_token_vect), error);
}

TEST_CASE("Test query parser with with clause") {
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

  // with clause
  test_token_vect.push_back(with_token);
  test_token_vect.push_back(attribute_statement_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(number_value_token_5);

  //BuildParsedQuery(test_token_vect);
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration();
  With with_clause = pq.GetWithClause().front();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(with_clause.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_clause.GetFirst().value == "s.stmt#");
  REQUIRE(with_clause.GetSecond().type == PqlTokenType::NUMBER);
  REQUIRE(with_clause.GetSecond().value == "5");
}

TEST_CASE("Test query parser with multiple with clauses") {
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

  // with clause
  test_token_vect.push_back(with_token);
  test_token_vect.push_back(attribute_statement_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(number_value_token_5);
  test_token_vect.push_back(with_token);
  test_token_vect.push_back(attribute_var_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(v1_token);

  //BuildParsedQuery(test_token_vect);
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration();
  With first_with_clause = pq.GetWithClause().front();
  With second_with_clause = pq.GetWithClause()[1];

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(first_with_clause.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(first_with_clause.GetFirst().value == "s.stmt#");
  REQUIRE(first_with_clause.GetSecond().type == PqlTokenType::NUMBER);
  REQUIRE(first_with_clause.GetSecond().value == "5");
  REQUIRE(second_with_clause.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(second_with_clause.GetFirst().value == "v.varName");
  REQUIRE(second_with_clause.GetSecond().type == PqlTokenType::SYNONYM);
  REQUIRE(second_with_clause.GetSecond().value == "v1");
}

TEST_CASE("Test query parser with BOOLEAN var name") {
  std::vector<PqlToken> test_token_vect;

  // stmt
  test_token_vect.push_back(stmt_token);
  test_token_vect.push_back(synonym_boolean_token);
  test_token_vect.push_back(semicolon_token);

  // variable
  test_token_vect.push_back(variable_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(semicolon_token);

  // select clause
  test_token_vect.push_back(select_token);
  test_token_vect.push_back(synonym_boolean_token);
  test_token_vect.push_back(such_token);
  test_token_vect.push_back(that_token);
  test_token_vect.push_back(use_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(closed_parenthesis_token);

  //BuildParsedQuery(test_token_vect);
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(synonym.value == "BOOLEAN");
  REQUIRE(decl.find("BOOLEAN")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
}