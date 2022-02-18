#include "catch.hpp"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/parsed_query.cpp"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"

TEST_CASE("Test query with uses and pattern") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Uses (s, \"x\") pattern a (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  Pattern ptrn = pq.GetPatterns().front();
  std::vector<Declaration> decl = pq.GetDeclaration();
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "\"x\"");
  REQUIRE(ptrn.GetSynAssign().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[1].GetSynonym().value == "v");
  REQUIRE(decl[2].GetDesignEntity().value == "assign");
  REQUIRE(decl[2].GetSynonym().value == "a");
}

TEST_CASE("Test query parser with more than one synonym") {
  std::string query = "stmt s1, s2; variable v1, v2; assign a1, a2, a3;\n Select s such that Uses (s, \"x\") pattern a (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  Relationship rship = pq.GetRelationships().front();
  Pattern ptrn = pq.GetPatterns().front();
  std::vector<Declaration> decl = pq.GetDeclaration();
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "\"x\"");
  REQUIRE(ptrn.GetSynAssign().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s1");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetSynonym().value == "s2");
  REQUIRE(decl[1].GetDesignEntity().value == "stmt");
  REQUIRE(decl[2].GetDesignEntity().value == "variable");
  REQUIRE(decl[2].GetSynonym().value == "v1");
  REQUIRE(decl[3].GetDesignEntity().value == "variable");
  REQUIRE(decl[3].GetSynonym().value == "v2");
  REQUIRE(decl[4].GetDesignEntity().value == "assign");
  REQUIRE(decl[4].GetSynonym().value == "a1");
  REQUIRE(decl[5].GetDesignEntity().value == "assign");
  REQUIRE(decl[5].GetSynonym().value == "a2");
  REQUIRE(decl[6].GetDesignEntity().value == "assign");
  REQUIRE(decl[6].GetSynonym().value == "a3");
}

TEST_CASE("Test query parser with no such that") {
  std::string query = "stmt s; variable v; assign a;\n Select s";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::vector<Declaration> decl = pq.GetDeclaration();
  std::vector<Relationship> rship = pq.GetRelationships();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  test that no relationships or patterns have been added
  REQUIRE(rship.size() == 0);
  REQUIRE(ptrns.size() == 0);
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[1].GetSynonym().value == "v");
  REQUIRE(decl[2].GetDesignEntity().value == "assign");
  REQUIRE(decl[2].GetSynonym().value == "a");
}

TEST_CASE("Test query parser with white spaces") {
  std::string query = "stmt s; variable v; assign a;\n Select    s  such that Modifies( 1,       v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::vector<Declaration> decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Test that no relationships or patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Modifies");
  REQUIRE(rship.GetFirst().value == "1");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[1].GetSynonym().value == "v");
  REQUIRE(decl[2].GetDesignEntity().value == "assign");
  REQUIRE(decl[2].GetSynonym().value == "a");
}

TEST_CASE("Test query parser with same variable referenced") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Parent*(s, s)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  std::vector<Declaration> decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Test that no patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Parent*");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "s");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl[0].GetSynonym().value == "s");
  REQUIRE(decl[0].GetDesignEntity().value == "stmt");
  REQUIRE(decl[1].GetDesignEntity().value == "variable");
  REQUIRE(decl[1].GetSynonym().value == "v");
  REQUIRE(decl[2].GetDesignEntity().value == "assign");
  REQUIRE(decl[2].GetSynonym().value == "a");
}
