#include "components/query_subsystem/pql/PqlToken.h"
#include "components/query_subsystem/pql/PqlToken.cpp"
#include "components/query_subsystem/pql/PqlLexer.h"
#include "components/query_subsystem/pql/PqlLexer.cpp"

#include "catch.hpp"
using namespace std;

TEST_CASE("Test Procedure Declaration") {
    PqlLexer pql_lexer = PqlLexer("procedure p;");
    vector<PqlToken> test_token_vect = pql_lexer.lex();

    vector<PqlToken> expected_token_vect;
    PqlToken procedure_token = PqlToken(TokenType::PROCEDURE, "procedure");
    PqlToken p_token = PqlToken(TokenType::SYNONYM, "p");
    PqlToken semicolon_token = PqlToken(TokenType::SEMICOLON, ";");
    expected_token_vect.push_back(procedure_token);
    expected_token_vect.push_back(p_token);
    expected_token_vect.push_back(semicolon_token);

    REQUIRE(test_token_vect == expected_token_vect);
}

