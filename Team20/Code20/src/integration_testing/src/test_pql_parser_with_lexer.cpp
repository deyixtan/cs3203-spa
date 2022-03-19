#include "catch.hpp"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"

TEST_CASE("Test query with uses and pattern") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Uses (s, \"x\") pattern a (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  Pattern ptrn = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration();
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "x");
  REQUIRE(ptrn.GetSynonym().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with more than one synonym") {
  std::string query = "stmt s1, s2; variable v1, v2; assign a1, a2, a3;\n Select s such that Uses (s, \"x\") pattern a (v, _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  Relationship rship = pq.GetRelationships().front();
  Pattern ptrn = pq.GetPatterns().front();
  const auto decl = pq.GetDeclaration();
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "x");
  REQUIRE(ptrn.GetSynonym().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(result_clause.GetValues()[0].value == "s");
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
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  std::vector<Relationship> rship = pq.GetRelationships();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  test that no relationships or patterns have been added
  REQUIRE(rship.size() == 0);
  REQUIRE(ptrns.size() == 0);
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with white spaces") {
  std::string query = "stmt s; variable v; assign a;\n Select    s  such that Modifies( 1,       v)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Test that no relationships or patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Modifies");
  REQUIRE(rship.GetFirst().value == "1");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with same variable referenced") {
  std::string query = "stmt s; variable v; assign a;\n Select s such that Parent*(s, s)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Test that no patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Parent*");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "s");
  REQUIRE(result_clause.GetValues()[0].value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with BOOLEAN as synonym name") {
  std::string query = "stmt BOOLEAN; variable v; assign a;\n Select BOOLEAN such that Parent*(s, 5)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
  std::vector<With> withs = pq.GetWithClause();
  // Test that no patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  // Test that no with clauses have been added
  REQUIRE(withs.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Parent*");
  REQUIRE(rship.GetFirst().value == "s");
  REQUIRE(rship.GetSecond().value == "5");
  REQUIRE(result_clause.GetValues()[0].value == "BOOLEAN");
  REQUIRE(result_clause.GetType() == ResultClauseType::SYNONYM);
  REQUIRE(decl.find("BOOLEAN")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with BOOLEAN and attributes") {
  std::string query = "stmt s; variable v; assign a;\n Select BOOLEAN such that Uses(5, v) with v.varName = \"bleh\"";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
  std::vector<With> withs = pq.GetWithClause();
  // Test that no patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "5");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "BOOLEAN");
  REQUIRE(withs[0].GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(withs[0].GetFirst().value == "v.varName");
  REQUIRE(withs[0].GetSecond().type == PqlTokenType::IDENT_WITH_QUOTES);
  REQUIRE(withs[0].GetSecond().value == "bleh");
  REQUIRE(result_clause.GetType() == ResultClauseType::BOOLEAN);
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with attributes") {
  std::string query = "stmt s; variable v; procedure p;\n Select BOOLEAN such that Uses(5, v) with v.varName = p.procName";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  Relationship rship = pq.GetRelationships().front();
  std::vector<Pattern> ptrns = pq.GetPatterns();
  std::vector<With> withs = pq.GetWithClause();
  // Test that no patterns have been added to query struct
  REQUIRE(ptrns.size() == 0);
  REQUIRE(rship.GetRelRef().value == "Uses");
  REQUIRE(rship.GetFirst().value == "5");
  REQUIRE(rship.GetSecond().value == "v");
  REQUIRE(result_clause.GetValues()[0].value == "BOOLEAN");
  REQUIRE(withs[0].GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(withs[0].GetFirst().value == "v.varName");
  REQUIRE(withs[0].GetSecond().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(withs[0].GetSecond().value == "p.procName");
  REQUIRE(result_clause.GetType() == ResultClauseType::BOOLEAN);
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("p")->second == DesignEntityType::PROCEDURE);
}

TEST_CASE("Test query parser with double pattern") {
  std::string query = "stmt s; variable v; assign a;\n Select a pattern a(v,_) and s(v, _\"x\"_)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
//  QueryValidator qv = QueryValidator(test_token_vect);
//  test_token_vect = qv.CheckValidation();
//  ParsedQueryBuilder pqb(test_token_vect);
//  ParsedQuery pq = pqb.Build();
//  ResultClause result_clause = pq.GetResultClause();
//  const auto decl = pq.GetDeclaration();
//  std::vector<Relationship> rship = pq.GetRelationships();
//  std::vector<Pattern> ptrns = pq.GetPatterns();
//  Pattern ptrn_assign = ptrns[0];
//  Pattern ptrn_stmt = ptrns[1];
//  std::vector<With> withs = pq.GetWithClause();
//  // Test that no relationships have been added to query struct
//  REQUIRE(rship.size() == 0);
//  REQUIRE(ptrn_assign.GetSynonym().value == "a");
//  REQUIRE(ptrn_assign.GetFirst().value == "v");
//  REQUIRE(ptrn_assign.GetSecond().value == "_");
//  REQUIRE(ptrn_stmt.GetSynonym().value == "s");
//  REQUIRE(ptrn_stmt.GetFirst().value == "v");
//  REQUIRE(ptrn_stmt.GetSecond().value == "x");
//  REQUIRE(result_clause.GetValues()[0].value == "a");
//  REQUIRE(result_clause.GetType() == ResultClauseType::SYNONYM);
//  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
//  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
//  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser with 'with' clause and 'and'") {
  std::string query = "stmt s; variable v; procedure p;\n Select a pattern a(v,_) with v.varName = p.procName and a.stmt# = 5";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  std::vector<Relationship> rship = pq.GetRelationships();
  Pattern ptrn = pq.GetPatterns().front();
  std::vector<With> withs = pq.GetWithClause();
  With with_first = withs.front();
  With with_second = withs[1];
  // Test that no relationships have been added to query struct
  REQUIRE(rship.size() == 0);
  REQUIRE(ptrn.GetSynonym().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(result_clause.GetValues()[0].value == "a");
  REQUIRE(with_first.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_first.GetFirst().value == "v.varName");
  REQUIRE(with_first.GetSecond().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_first.GetSecond().value == "p.procName");
  REQUIRE(with_second.GetFirst().type == PqlTokenType::ATTRIBUTE);
  REQUIRE(with_second.GetFirst().value == "a.stmt#");
  REQUIRE(with_second.GetSecond().type == PqlTokenType::NUMBER);
  REQUIRE(with_second.GetSecond().value == "5");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "_");
  REQUIRE(result_clause.GetType() == ResultClauseType::SYNONYM);
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("p")->second == DesignEntityType::PROCEDURE);
}

TEST_CASE("Test query parser with 'such that' clause and 'and'") {
  std::string query = "stmt s; variable v; assign a;\n Select a such that Uses(s, v) and Parent(a, s)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  std::vector<Relationship> rship = pq.GetRelationships();
  Relationship rship_uses = rship.front();
  Relationship rship_parent = rship[1];
  std::vector<With> withs = pq.GetWithClause();
  // Test that no with clauses have been added to query struct
  REQUIRE(withs.size() == 0);
  REQUIRE(rship_uses.GetRelRef().value == "Uses");
  REQUIRE(rship_uses.GetFirst().value == "s");
  REQUIRE(rship_uses.GetSecond().value == "v");
  REQUIRE(rship_parent.GetRelRef().value == "Parent");
  REQUIRE(rship_parent.GetFirst().value == "a");
  REQUIRE(rship_parent.GetSecond().value == "s");
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("a")->second == DesignEntityType::ASSIGN);
}

TEST_CASE("Test query parser sub-expression with whitespaces") {
  std::string query = "stmt s; variable v; procedure p;\n Select a pattern a(v, _  \"1 * red\"   _)";
  PqlLexer pql_lexer = PqlLexer(query);
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  QueryValidator qv = QueryValidator(test_token_vect);
  test_token_vect = qv.CheckValidation();
  ParsedQueryBuilder pqb(test_token_vect);
  ParsedQuery pq = pqb.Build();
  ResultClause result_clause = pq.GetResultClause();
  const auto decl = pq.GetDeclaration();
  std::vector<Relationship> rship = pq.GetRelationships();
  Pattern ptrn = pq.GetPatterns().front();
  // Test that no relationships have been added to query struct
  REQUIRE(rship.size() == 0);
  REQUIRE(ptrn.GetSynonym().value == "a");
  REQUIRE(ptrn.GetFirst().value == "v");
  REQUIRE(ptrn.GetSecond().value == "1*red");
  REQUIRE(result_clause.GetValues()[0].value == "a");
  REQUIRE(result_clause.GetType() == ResultClauseType::SYNONYM);
  REQUIRE(decl.find("s")->second == DesignEntityType::STMT);
  REQUIRE(decl.find("v")->second == DesignEntityType::VARIABLE);
  REQUIRE(decl.find("p")->second == DesignEntityType::PROCEDURE);
}
