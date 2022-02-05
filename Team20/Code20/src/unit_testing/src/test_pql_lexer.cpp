#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"

#include "catch.hpp"

PqlToken comma_token = PqlToken(TokenType::COMMA, ",");
PqlToken semicolon_token = PqlToken(TokenType::SEMICOLON, ";");
PqlToken open_parenthesis_token = PqlToken(TokenType::OPEN_PARENTHESIS, "(");
PqlToken closed_parenthesis_token = PqlToken(TokenType::CLOSED_PARENTHESIS, ")");
PqlToken double_quote_token = PqlToken(TokenType::DOUBLE_QUOTE, "\"");
PqlToken underscore_token = PqlToken(TokenType::UNDERSCORE, "_");
PqlToken plus_token = PqlToken(TokenType::PLUS, "+");

PqlToken stmt_token = PqlToken(TokenType::STMT, "stmt");
PqlToken assign_token = PqlToken(TokenType::ASSIGN, "assign");
PqlToken while_token = PqlToken(TokenType::WHILE, "while");
PqlToken if_token = PqlToken(TokenType::IF, "if");
PqlToken variable_token = PqlToken(TokenType::VARIABLE, "variable");
PqlToken procedure_token = PqlToken(TokenType::PROCEDURE, "procedure");
PqlToken constant_token = PqlToken(TokenType::CONSTANT, "constant");
PqlToken read_token = PqlToken(TokenType::READ, "read");
PqlToken print_token = PqlToken(TokenType::PRINT, "print");
PqlToken call_token = PqlToken(TokenType::CALL, "call");

PqlToken s_token = PqlToken(TokenType::SYNONYM, "s");
PqlToken s1_token = PqlToken(TokenType::SYNONYM, "s1");
PqlToken a_token = PqlToken(TokenType::SYNONYM, "a");
PqlToken a1_token = PqlToken(TokenType::SYNONYM, "a1");
PqlToken w_token = PqlToken(TokenType::SYNONYM, "w");
PqlToken ifs_token = PqlToken(TokenType::SYNONYM, "ifs");
PqlToken v_token = PqlToken(TokenType::SYNONYM, "v");
PqlToken v1_token = PqlToken(TokenType::SYNONYM, "v1");
PqlToken p_token = PqlToken(TokenType::SYNONYM, "p");
PqlToken q_token = PqlToken(TokenType::SYNONYM, "q");
PqlToken c_token = PqlToken(TokenType::SYNONYM, "c");
PqlToken re_token = PqlToken(TokenType::SYNONYM, "re");
PqlToken pn_token = PqlToken(TokenType::SYNONYM, "pn");
PqlToken cl_token = PqlToken(TokenType::SYNONYM, "cl");

PqlToken number_value_token = PqlToken(TokenType::NUMBER, "1");
PqlToken x_token = PqlToken(TokenType::SYNONYM, "x");
PqlToken y_token = PqlToken(TokenType::SYNONYM, "y");
PqlToken x_string_token = PqlToken(TokenType::IDENT_WITH_QUOTES, "\"x\"");
PqlToken x_plus_y_string_token = PqlToken(TokenType::EXPR, "\"x+y\"");
PqlToken sub_expression_token = PqlToken(TokenType::SUB_EXPRESSION, "_\"x+y\"_");

PqlToken select_token = PqlToken(TokenType::SELECT, "Select");
PqlToken such_token = PqlToken(TokenType::SUCH, "such");
PqlToken that_token = PqlToken(TokenType::THAT, "that");

PqlToken follow_token = PqlToken(TokenType::FOLLOWS, "Follows");
PqlToken follow_t_token = PqlToken(TokenType::FOLLOWS_T, "Follows*");
PqlToken parent_token = PqlToken(TokenType::PARENT, "Parent");
PqlToken parent_t_token = PqlToken(TokenType::PARENT_T, "Parent*");
PqlToken use_token = PqlToken(TokenType::USES, "Uses");
PqlToken modify_token = PqlToken(TokenType::MODIFIES, "Modifies");
PqlToken pattern_token = PqlToken(TokenType::PATTERN, "pattern");

TEST_CASE("Test single declaration") {
  PqlLexer pql_lexer = PqlLexer("procedure p;");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();

  std::vector<PqlToken> expected_token_vect;
  expected_token_vect.push_back(procedure_token);
  expected_token_vect.push_back(p_token);
  expected_token_vect.push_back(semicolon_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test multiple declarations") {
  PqlLexer pql_lexer = PqlLexer("stmt s, s1; assign a, a1; while w; if ifs; variable v, v1;\n"
                                "procedure p, q; constant c; read re; print pn; call cl;");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(s1_token);
  expected_token_vect.push_back(semicolon_token);

  // assign
  expected_token_vect.push_back(assign_token);
  expected_token_vect.push_back(a_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(a1_token);
  expected_token_vect.push_back(semicolon_token);

  // while
  expected_token_vect.push_back(while_token);
  expected_token_vect.push_back(w_token);
  expected_token_vect.push_back(semicolon_token);

  // if
  expected_token_vect.push_back(if_token);
  expected_token_vect.push_back(ifs_token);
  expected_token_vect.push_back(semicolon_token);

  // variable
  expected_token_vect.push_back(variable_token);
  expected_token_vect.push_back(v_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(v1_token);
  expected_token_vect.push_back(semicolon_token);

  // procedure
  expected_token_vect.push_back(procedure_token);
  expected_token_vect.push_back(p_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(q_token);
  expected_token_vect.push_back(semicolon_token);

  // constant
  expected_token_vect.push_back(constant_token);
  expected_token_vect.push_back(c_token);
  expected_token_vect.push_back(semicolon_token);

  // read
  expected_token_vect.push_back(read_token);
  expected_token_vect.push_back(re_token);
  expected_token_vect.push_back(semicolon_token);

  // print
  expected_token_vect.push_back(print_token);
  expected_token_vect.push_back(pn_token);
  expected_token_vect.push_back(semicolon_token);

  // call
  expected_token_vect.push_back(call_token);
  expected_token_vect.push_back(cl_token);
  expected_token_vect.push_back(semicolon_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with follows relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s;\n"
                                "Select s such that Follows (1, s)");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(semicolon_token);

  // select clause
  expected_token_vect.push_back(select_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(such_token);
  expected_token_vect.push_back(that_token);
  expected_token_vect.push_back(follow_token);
  expected_token_vect.push_back(open_parenthesis_token);
  expected_token_vect.push_back(number_value_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with follows* relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s;\n"
                                "Select s such that Follows* (1, s)");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(semicolon_token);

  // select clause
  expected_token_vect.push_back(select_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(such_token);
  expected_token_vect.push_back(that_token);
  expected_token_vect.push_back(follow_t_token);
  expected_token_vect.push_back(open_parenthesis_token);
  expected_token_vect.push_back(number_value_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with parent relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s;\n"
                                "Select s such that Parent (1, s)");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(semicolon_token);

  // select clause
  expected_token_vect.push_back(select_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(such_token);
  expected_token_vect.push_back(that_token);
  expected_token_vect.push_back(parent_token);
  expected_token_vect.push_back(open_parenthesis_token);
  expected_token_vect.push_back(number_value_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with parent* relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s;\n"
                                "Select s such that Parent* (1, s)");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(semicolon_token);

  // select clause
  expected_token_vect.push_back(select_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(such_token);
  expected_token_vect.push_back(that_token);
  expected_token_vect.push_back(parent_t_token);
  expected_token_vect.push_back(open_parenthesis_token);
  expected_token_vect.push_back(number_value_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with use relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s;\n"
                                "Select s such that Uses (s, \"x\")");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
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

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with modify relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s;\n"
                                "Select s such that Modifies (s, \"x\")");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
  std::vector<PqlToken> expected_token_vect;

  // stmt
  expected_token_vect.push_back(stmt_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(semicolon_token);

  // select clause
  expected_token_vect.push_back(select_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(such_token);
  expected_token_vect.push_back(that_token);
  expected_token_vect.push_back(modify_token);
  expected_token_vect.push_back(open_parenthesis_token);
  expected_token_vect.push_back(s_token);
  expected_token_vect.push_back(comma_token);
  expected_token_vect.push_back(x_string_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with pattern relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s; variable v; assign a;\n"
                                "Select s such that Uses (s, \"x\") pattern a (v, \"x + y\")");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
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
  expected_token_vect.push_back(x_plus_y_string_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with pattern relationship omitting spaces") {
PqlLexer pql_lexer = PqlLexer("stmt s; variable v; assign a;\n"
                              "Select s such that Uses (s,\"x\") pattern a (v,\"x+ y\")");
std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
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
expected_token_vect.push_back(x_plus_y_string_token);
expected_token_vect.push_back(closed_parenthesis_token);

REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with pattern sub-expression relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s; variable v; assign a;\n"
                                "Select s such that Uses (s, \"x\") pattern a (v, _\"x + y\"_)");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
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
  expected_token_vect.push_back(sub_expression_token);
  expected_token_vect.push_back(closed_parenthesis_token);

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with pattern underscore relationship") {
  PqlLexer pql_lexer = PqlLexer("stmt s; variable v; assign a;\n"
                                "Select s such that Uses (s, \"x\") pattern a (v, _)");
  std::vector<PqlToken> test_token_vect = pql_lexer.Lex();
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

  REQUIRE(test_token_vect == expected_token_vect);
}

TEST_CASE("Test declarations with incomplete double quote token") {
  PqlLexer pql_lexer = PqlLexer("stmt s; variable v;\n"
                                "Select s such that Uses (s, \"x\") pattern a (\"x\", \"y)");
  std::string expected_wrong_token = "\"y)";
  REQUIRE_THROWS_WITH(pql_lexer.Lex(), "ERROR: Unrecognised token " + expected_wrong_token + "\n");
}

TEST_CASE("Test invalid string") {
  PqlLexer pql_lexer = PqlLexer("Select s such that Uses (s, \"x\") pattern a (_, \"+y\")");
  std::string expected_wrong_token = "\"+y\"";
  REQUIRE_THROWS_WITH(pql_lexer.Lex(), "ERROR: Unrecognised token " + expected_wrong_token + "\n");
}

TEST_CASE("Test invalid sub-expression") {
  PqlLexer pql_lexer = PqlLexer("Select s such that Uses (s, \"x\") pattern a (_, _\"y\"x_)");
  std::string expected_wrong_token = "_\"y\"x_";
  REQUIRE_THROWS_WITH(pql_lexer.Lex(), "ERROR: Unrecognised token " + expected_wrong_token + "\n");
}

TEST_CASE("Test declarations with unrecognised token") {
  PqlLexer pql_lexer = PqlLexer("stmt s; variable v;\n"
                                "Select s such that Uses (s, \"x\") && Modifies (s, v)");
  std::string expected_wrong_token = "&&";

  REQUIRE_THROWS_WITH(pql_lexer.Lex(), "ERROR: Unrecognised token " + expected_wrong_token + "\n");
}
