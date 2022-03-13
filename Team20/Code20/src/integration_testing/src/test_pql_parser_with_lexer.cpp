#include "catch.hpp"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
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
  const auto decl = pq.GetDeclaration();
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "\"x\"");
  REQUIRE(ptrn.GetSynAssign().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
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
  const auto decl = pq.GetDeclaration();
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "\"x\"");
  REQUIRE(ptrn.GetSynAssign().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s1")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("s2")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v1")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("v2")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a1")->second == DesignEntityType::ASSIGN);
  REQUIRE(decl.find("a2")->second == DesignEntityType::ASSIGN);
  REQUIRE(decl.find("a3")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with no such that") {
  std::string query = "stmt s; variable v; assign a;\n Select s";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  const auto decl = pq.GetDeclaration();
  std::vector<Relationship> rship = pq.GetRelationships();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  test that no relationships or patterns have been added
  REQUIRE(rship.size() == 0);
  REQUIRE(ptrns.size() == 0);
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with white spaces") {
  std::string query = "stmt s; variable v; assign a;\n Select    s  such that Modifies( 1,       v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Test that no relationships or patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Modifies");
  REQUIRE(rship.GetFirst().value == "1");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with same variable referenced") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Parent*(s, s)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb;
  ParsedQuery pq = pqb.Build(test_token_vect);
  PqlToken synonym = pq.GetSynonym();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Test that no patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Parent*");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "s");
  REQUIRE(synonym.value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}
