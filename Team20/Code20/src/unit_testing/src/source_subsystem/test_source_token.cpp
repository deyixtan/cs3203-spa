#include "catch.hpp"
#include "components/source_subsystem/types/source_token/source_token.h"

using namespace source;

TEST_CASE("Test UNKNOWN SourceToken") {
  SourceToken token = SourceToken(TokenType::UNKNOWN, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::UNKNOWN);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test WHITE_SPACE SourceToken") {
  SourceToken token = SourceToken(TokenType::WHITE_SPACE, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::WHITE_SPACE);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test NEW_LINE SourceToken") {
  SourceToken token = SourceToken(TokenType::NEW_LINE, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::NEW_LINE);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test TAB SourceToken") {
  SourceToken token = SourceToken(TokenType::TAB, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::TAB);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test INTEGER SourceToken") {
  SourceToken token = SourceToken(TokenType::INTEGER, "123456");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::INTEGER);
  REQUIRE(value == "123456");
}

TEST_CASE("Test NAME SourceToken") {
  SourceToken token = SourceToken(TokenType::NAME, "my_var");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::NAME);
  REQUIRE(value == "my_var");
}

TEST_CASE("Test OPENED_BRACES SourceToken") {
  SourceToken token = SourceToken(TokenType::OPENED_BRACES, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::OPENED_BRACES);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test CLOSED_BRACES SourceToken") {
  SourceToken token = SourceToken(TokenType::CLOSED_BRACES, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::CLOSED_BRACES);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test OPENED_PARENTHESIS SourceToken") {
  SourceToken token = SourceToken(TokenType::OPENED_PARENTHESIS, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::OPENED_PARENTHESIS);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test CLOSED_PARENTHESIS SourceToken") {
  SourceToken token = SourceToken(TokenType::CLOSED_PARENTHESIS, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::CLOSED_PARENTHESIS);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test NOT SourceToken") {
  SourceToken token = SourceToken(TokenType::NOT, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::NOT);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test AND SourceToken") {
  SourceToken token = SourceToken(TokenType::AND, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::AND);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test OR SourceToken") {
  SourceToken token = SourceToken(TokenType::OR, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::OR);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IS_EQUAL SourceToken") {
  SourceToken token = SourceToken(TokenType::IS_EQUAL, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IS_EQUAL);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IS_NOT_EQUAL SourceToken") {
  SourceToken token = SourceToken(TokenType::IS_NOT_EQUAL, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IS_NOT_EQUAL);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IS_GREATER_EQUAL SourceToken") {
  SourceToken token = SourceToken(TokenType::IS_GREATER_EQUAL, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IS_GREATER_EQUAL);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IS_LESSER_EQUAL SourceToken") {
  SourceToken token = SourceToken(TokenType::IS_LESSER_EQUAL, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IS_LESSER_EQUAL);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IS_GREATER SourceToken") {
  SourceToken token = SourceToken(TokenType::IS_GREATER, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IS_GREATER);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IS_LESSER SourceToken") {
  SourceToken token = SourceToken(TokenType::IS_LESSER, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IS_LESSER);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test ADDITION SourceToken") {
  SourceToken token = SourceToken(TokenType::ADDITION, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::ADDITION);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test SUBTRACTION SourceToken") {
  SourceToken token = SourceToken(TokenType::SUBTRACTION, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::SUBTRACTION);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test MULTIPLICATION SourceToken") {
  SourceToken token = SourceToken(TokenType::MULTIPLICATION, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::MULTIPLICATION);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test DIVISION SourceToken") {
  SourceToken token = SourceToken(TokenType::DIVISION, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::DIVISION);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test MODULUS SourceToken") {
  SourceToken token = SourceToken(TokenType::MODULUS, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::MODULUS);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test EQUAL SourceToken") {
  SourceToken token = SourceToken(TokenType::EQUAL, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::EQUAL);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test SEMI_COLON SourceToken") {
  SourceToken token = SourceToken(TokenType::SEMI_COLON, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::SEMI_COLON);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test PROCEDURE SourceToken") {
  SourceToken token = SourceToken(TokenType::PROCEDURE, "main");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::PROCEDURE);
  REQUIRE(value == "main");
}

TEST_CASE("Test READ SourceToken") {
  SourceToken token = SourceToken(TokenType::READ, "x");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::READ);
  REQUIRE(value == "x");
}

TEST_CASE("Test PRINT SourceToken") {
  SourceToken token = SourceToken(TokenType::PRINT, "x");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::PRINT);
  REQUIRE(value == "x");
}

TEST_CASE("Test CALL SourceToken") {
  SourceToken token = SourceToken(TokenType::CALL, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::CALL);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test WHILE SourceToken") {
  SourceToken token = SourceToken(TokenType::WHILE, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::WHILE);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test IF SourceToken") {
  SourceToken token = SourceToken(TokenType::IF, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::IF);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test THEN SourceToken") {
  SourceToken token = SourceToken(TokenType::THEN, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::THEN);
  REQUIRE(std::empty(value));
}

TEST_CASE("Test ELSE SourceToken") {
  SourceToken token = SourceToken(TokenType::ELSE, "");
  TokenType type = token.GetType();
  std::string value = token.GetValue();

  REQUIRE(type == TokenType::ELSE);
  REQUIRE(std::empty(value));
}
