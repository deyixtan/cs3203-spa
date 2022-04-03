#include "sample_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.cpp"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_parser/pql_parser.h"

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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "s");
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  Pattern patt = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "x");
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(patt.GetSynonym().value == "a");
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(rship.GetRelRef().value == "Parent");
  REQUIRE(rship.GetFirst().value == "1");
  REQUIRE(rship.GetSecond().value == "s");
  REQUIRE(result_clause.GetValues()[0].value == "s");
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(result_clause.GetValues()[0].value == "s");
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  std::optional<Pattern> patt = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(result_clause.GetValues()[0].value == "a");
  REQUIRE(patt->GetSynonym().value == "a");
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

  std::string INVALID_QUERY_FORMAT = "Invalid Query Format! \n";

  PqlParser pql_parser = PqlParser(test_token_vect);
  std::string error = INVALID_QUERY_FORMAT;

  REQUIRE_THROWS_WITH(pql_parser.ParseQuery(), error);
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();
  With with_clause = pq.GetWithClause().front();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "s");
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();
  With first_with_clause = pq.GetWithClause().front();
  With second_with_clause = pq.GetWithClause()[1];

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "s");
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

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "BOOLEAN");
  REQUIRE(decl.find("BOOLEAN")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
}

TEST_CASE("Test query parser with ifs pattern") {
  std::vector<PqlToken> test_token_vect;

  // if
  test_token_vect.push_back(if_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(semicolon_token);

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
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(pattern_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Pattern patt_ifs = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(decl.find("ifs")->second == DesignEntityType::IF);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(patt_ifs.GetFirst().type == PqlTokenType::SYNONYM);
  REQUIRE(patt_ifs.GetSecond().type == PqlTokenType::UNDERSCORE);
  REQUIRE(patt_ifs.GetThird().type == PqlTokenType::UNDERSCORE);
}

TEST_CASE("Test query parser with And clause") {
  std::vector<PqlToken> test_token_vect;

  // if
  test_token_vect.push_back(if_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(semicolon_token);

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
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(pattern_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  // and clause
  test_token_vect.push_back(and_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Pattern patt_ifs = pq.GetPatterns().front();
  Pattern patt_assign = pq.GetPatterns()[1];
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(decl.find("ifs")->second == DesignEntityType::IF);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(patt_ifs.GetFirst().type == PqlTokenType::SYNONYM);
  REQUIRE(patt_ifs.GetSecond().type == PqlTokenType::UNDERSCORE);
  REQUIRE(patt_ifs.GetThird().type == PqlTokenType::UNDERSCORE);
  REQUIRE(patt_assign.GetFirst().type == PqlTokenType::SYNONYM);
  REQUIRE(patt_assign.GetSecond().type == PqlTokenType::UNDERSCORE);
}

TEST_CASE("Test query parser with attributes and values") {
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
  test_token_vect.push_back(number_value_token_5);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(attribute_statement_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();
  With with_clause = pq.GetWithClause().front();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(with_clause.GetFirst().type == PqlTokenType::NUMBER);
  REQUIRE(with_clause.GetFirst().value == "5");
  REQUIRE(with_clause.GetSecond().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_clause.GetSecond().value == "s.stmt#");
}

TEST_CASE("Test query parser with attributes and value strings") {
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
  test_token_vect.push_back(attribute_var_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(v_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  const auto decl = pq.GetDeclaration().GetDeclarations();
  With with_clause = pq.GetWithClause().front();

  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(with_clause.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_clause.GetFirst().value == "v.varName");
  REQUIRE(with_clause.GetSecond().type == PqlTokenType::SYNONYM);
  REQUIRE(with_clause.GetSecond().value == "v");
}

TEST_CASE("Test query parser with combination of attributes") {
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
  test_token_vect.push_back(with_token);
  test_token_vect.push_back(attribute_statement_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(attribute_const_token);
  test_token_vect.push_back(and_token);
  test_token_vect.push_back(attribute_statement_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(number_value_token_2);
  test_token_vect.push_back(and_token);
  test_token_vect.push_back(v1_token);
  test_token_vect.push_back(equal_sign_token);
  test_token_vect.push_back(attribute_var_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration().GetDeclarations();
  With with_first = pq.GetWithClause().front();
  With with_second = pq.GetWithClause()[1];
  With with_third = pq.GetWithClause()[2];

  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(with_first.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_first.GetFirst().value == "s.stmt#");
  REQUIRE(with_first.GetSecond().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_first.GetSecond().value == "c.value");
  REQUIRE(with_second.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_second.GetFirst().value == "s.stmt#");
  REQUIRE(with_second.GetSecond().type == PqlTokenType::NUMBER);
  REQUIRE(with_second.GetSecond().value == "2");
  REQUIRE(with_third.GetFirst().type == PqlTokenType::SYNONYM);
  REQUIRE(with_third.GetFirst().value == "v1");
  REQUIRE(with_third.GetSecond().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_third.GetSecond().value == "v.varName");
}

TEST_CASE("Test query parser with multiple And clauses") {
  std::vector<PqlToken> test_token_vect;

  // if
  test_token_vect.push_back(if_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(semicolon_token);

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
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(pattern_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  // and clause
  test_token_vect.push_back(and_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(v_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  test_token_vect.push_back(such_token);
  test_token_vect.push_back(that_token);
  test_token_vect.push_back(affects_token);
  test_token_vect.push_back(closed_parenthesis_token);
  test_token_vect.push_back(a1_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(a_token);
  test_token_vect.push_back(closed_parenthesis_token);
  test_token_vect.push_back(and_token);
  test_token_vect.push_back(parent_token);
  test_token_vect.push_back(open_parenthesis_token);
  test_token_vect.push_back(s_token);
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(s1_token);
  test_token_vect.push_back(closed_parenthesis_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  ParsedQuery pq = pql_parser.ParseQuery();
  ResultClause result_clause = pq.GetResultClause();
  Pattern patt_ifs = pq.GetPatterns().front();
  Pattern patt_assign = pq.GetPatterns()[1];
  Relationship rship_first = pq.GetRelationships().front();
  Relationship rship_second = pq.GetRelationships()[1];
  const auto decl = pq.GetDeclaration().GetDeclarations();

  REQUIRE(decl.find("ifs")->second == DesignEntityType::IF);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(patt_ifs.GetFirst().type == PqlTokenType::SYNONYM);
  REQUIRE(patt_ifs.GetSecond().type == PqlTokenType::UNDERSCORE);
  REQUIRE(patt_ifs.GetThird().type == PqlTokenType::UNDERSCORE);
  REQUIRE(patt_assign.GetFirst().type == PqlTokenType::SYNONYM);
  REQUIRE(patt_assign.GetSecond().type == PqlTokenType::UNDERSCORE);
  REQUIRE(rship_first.GetRelRef().value == "Affects");
  REQUIRE(rship_first.GetFirst().value == "a1");
  REQUIRE(rship_first.GetSecond().value == "a");
  REQUIRE(rship_second.GetRelRef().value == "Parent");
  REQUIRE(rship_second.GetFirst().value == "s");
  REQUIRE(rship_second.GetSecond().value == "s1");
}

TEST_CASE("Test invalid pattern with extra args") {
  std::vector<PqlToken> test_token_vect;

  // if
  test_token_vect.push_back(if_token);
  test_token_vect.push_back(ifs_token);
  test_token_vect.push_back(semicolon_token);

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
  test_token_vect.push_back(comma_token);
  test_token_vect.push_back(underscore_token);
  test_token_vect.push_back(closed_parenthesis_token);

  PqlParser pql_parser = PqlParser(test_token_vect);
  std::string INVALID_QUERY_FORMAT = "Invalid Query Format! \n";
  std::string error = INVALID_QUERY_FORMAT;

  REQUIRE_THROWS_WITH(pql_parser.ParseQuery(), error);
}