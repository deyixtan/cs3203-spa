#include "sample_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.cpp"
#include "components/query_subsystem/pql_parser/parsed_query.h"

#include "catch.hpp"

TEST_CASE("Test query parser") {
std::vector<PqlToken> expected_token_vect;

// stmt
expected_token_vect.push_back(stmt_token);
expected_token_vect.push_back(s_token);
expected_token_vect.push_back(semicolon_token);

// variable
expected_token_vect.push_back(variable_token);
expected_token_vect.push_back(v_token);
expected_token_vect.push_back(semicolon_token);

// assign
expected_token_vect.push_back(assign_token);
expected_token_vect.push_back(a_token);
expected_token_vect.push_back(semicolon_token);

// select clause
expected_token_vect.push_back(select_token);
expected_token_vect.push_back(s_token);
expected_token_vect.push_back(such_token);
expected_token_vect.push_back(that_token);
expected_token_vect.push_back(use_token);
expected_token_vect.push_back(open_parenthesis_token);
expected_token_vect.push_back(s_token);
expected_token_vect.push_back(comma_token);
expected_token_vect.push_back(x_string_token);
expected_token_vect.push_back(closed_parenthesis_token);
expected_token_vect.push_back(pattern_token);
expected_token_vect.push_back(a_token);
expected_token_vect.push_back(open_parenthesis_token);
expected_token_vect.push_back(v_token);
expected_token_vect.push_back(comma_token);
expected_token_vect.push_back(underscore_token);
expected_token_vect.push_back(closed_parenthesis_token);

//BuildParsedQuery(expected_token_vect);
ParsedQuery pq = BuildParsedQuery(expected_token_vect);
PqlToken synonym = pq.GetSynonym();
Relationship rship = pq.GetRelationship();
Pattern patt = pq.GetPattern();
std::vector<Declaration> decl = pq.GetDeclaration();
REQUIRE(rship.GetRelRef().value == "Uses");
REQUIRE(rship.GetFirst().value == "s");
REQUIRE(rship.GetSecond().value == "\"x\"");
REQUIRE(synonym.value == "s");
REQUIRE(patt.GetSynAssign().value == "a");
REQUIRE(patt.GetFirst().value == "v");
REQUIRE(patt.GetSecond().value == "_");
REQUIRE(decl[0].GetSynonym().value == "s");
REQUIRE(decl[0].GetDesignEntity().value == "stmt");
REQUIRE(decl[1].GetDesignEntity().value == "variable");
REQUIRE(decl[1].GetSynonym().value == "v");
REQUIRE(decl[2].GetDesignEntity().value == "assign");
REQUIRE(decl[2].GetSynonym().value == "a");
}