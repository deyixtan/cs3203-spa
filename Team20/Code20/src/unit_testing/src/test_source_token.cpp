#include "components/source_subsystem/source_token.h"
#include "catch.hpp"

TEST_CASE("Test source WHITESPACE token") {
  source::SourceToken token = source::SourceToken(source::TokenType::WHITE_SPACE, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::WHITE_SPACE);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: WHITE_SPACE, value: }");
}

TEST_CASE("Test source INTEGER token") {
  source::SourceToken token = source::SourceToken(source::TokenType::INTEGER, "123");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::INTEGER);
  REQUIRE(value == "123");
  REQUIRE(typeStr == "{ type: INTEGER, value: 123}");
}

TEST_CASE("Test source NAME token") {
  source::SourceToken token = source::SourceToken(source::TokenType::NAME, "x");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::NAME);
  REQUIRE(value == "x");
  REQUIRE(typeStr == "{ type: NAME, value: x}");
}

TEST_CASE("Test source OPENED_BRACES token") {
  source::SourceToken token = source::SourceToken(source::TokenType::OPENED_BRACES, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::OPENED_BRACES);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: OPENED_BRACES, value: }");
}

TEST_CASE("Test source CLOSED_BRACES token") {
  source::SourceToken token = source::SourceToken(source::TokenType::CLOSED_BRACES, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::CLOSED_BRACES);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: CLOSED_BRACES, value: }");
}

TEST_CASE("Test source OPENED_PARENTHESIS token") {
  source::SourceToken token = source::SourceToken(source::TokenType::OPENED_PARENTHESIS, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::OPENED_PARENTHESIS);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: OPENED_PARENTHESIS, value: }");
}

TEST_CASE("Test source CLOSED_PARENTHESIS token") {
  source::SourceToken token = source::SourceToken(source::TokenType::CLOSED_PARENTHESIS, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::CLOSED_PARENTHESIS);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: CLOSED_PARENTHESIS, value: }");
}

TEST_CASE("Test source NOT token") {
  source::SourceToken token = source::SourceToken(source::TokenType::NOT, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::NOT);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: NOT, value: }");
}

TEST_CASE("Test source AND token") {
  source::SourceToken token = source::SourceToken(source::TokenType::AND, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::AND);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: AND, value: }");
}

TEST_CASE("Test source OR token") {
  source::SourceToken token = source::SourceToken(source::TokenType::OR, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::OR);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: OR, value: }");
}

TEST_CASE("Test source IS_GREATER token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IS_GREATER, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IS_GREATER);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IS_GREATER, value: }");
}

TEST_CASE("Test source IS_GREATER_EQUAL token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IS_GREATER_EQUAL, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IS_GREATER_EQUAL);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IS_GREATER_EQUAL, value: }");
}

TEST_CASE("Test source IS_LESSER token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IS_LESSER, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IS_LESSER);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IS_LESSER, value: }");
}

TEST_CASE("Test source IS_LESSER_EQUAL token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IS_LESSER_EQUAL, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IS_LESSER_EQUAL);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IS_LESSER_EQUAL, value: }");
}

TEST_CASE("Test source IS_EQUAL token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IS_EQUAL, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IS_EQUAL);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IS_EQUAL, value: }");
}

TEST_CASE("Test source IS_NOT_EQUAL token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IS_NOT_EQUAL, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IS_NOT_EQUAL);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IS_NOT_EQUAL, value: }");
}

TEST_CASE("Test source ADDITION token") {
  source::SourceToken token = source::SourceToken(source::TokenType::ADDITION, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::ADDITION);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: ADDITION, value: }");
}

TEST_CASE("Test source SUBTRACTION token") {
  source::SourceToken token = source::SourceToken(source::TokenType::SUBTRACTION, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::SUBTRACTION);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: SUBTRACTION, value: }");
}

TEST_CASE("Test source MULTIPLICATION token") {
  source::SourceToken token = source::SourceToken(source::TokenType::MULTIPLICATION, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::MULTIPLICATION);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: MULTIPLICATION, value: }");
}

TEST_CASE("Test source DIVISION token") {
  source::SourceToken token = source::SourceToken(source::TokenType::DIVISION, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::DIVISION);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: DIVISION, value: }");
}

TEST_CASE("Test source MODULUS token") {
  source::SourceToken token = source::SourceToken(source::TokenType::MODULUS, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::MODULUS);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: MODULUS, value: }");
}

TEST_CASE("Test source EQUAL token") {
  source::SourceToken token = source::SourceToken(source::TokenType::EQUAL, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::EQUAL);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: EQUAL, value: }");
}

TEST_CASE("Test source SEMI_COLON token") {
  source::SourceToken token = source::SourceToken(source::TokenType::SEMI_COLON, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::SEMI_COLON);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: SEMI_COLON, value: }");
}

TEST_CASE("Test source READ token") {
  source::SourceToken token = source::SourceToken(source::TokenType::READ, "x");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::READ);
  REQUIRE(value == "x");
  REQUIRE(typeStr == "{ type: READ, value: x}");
}

TEST_CASE("Test source PRINT token") {
  source::SourceToken token = source::SourceToken(source::TokenType::PRINT, "x");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::PRINT);
  REQUIRE(value == "x");
  REQUIRE(typeStr == "{ type: PRINT, value: x}");
}

TEST_CASE("Test source WHILE token") {
  source::SourceToken token = source::SourceToken(source::TokenType::WHILE, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::WHILE);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: WHILE, value: }");
}

TEST_CASE("Test source IF token") {
  source::SourceToken token = source::SourceToken(source::TokenType::IF, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::IF);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: IF, value: }");
}

TEST_CASE("Test source THEN token") {
  source::SourceToken token = source::SourceToken(source::TokenType::THEN, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::THEN);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: THEN, value: }");
}

TEST_CASE("Test source ELSE token") {
  source::SourceToken token = source::SourceToken(source::TokenType::ELSE, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::ELSE);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: ELSE, value: }");
}

TEST_CASE("Test source PROCEDURE token") {
  source::SourceToken token = source::SourceToken(source::TokenType::PROCEDURE, "main");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::PROCEDURE);
  REQUIRE(value == "main");
  REQUIRE(typeStr == "{ type: PROCEDURE, value: main}");
}

TEST_CASE("Test source UNKNOWN token") {
  source::SourceToken token = source::SourceToken(source::TokenType::UNKNOWN, "");
  source::TokenType type = token.GetType();
  std::string value = token.GetValue();
  std::string typeStr = token.ToString();

  REQUIRE(type == source::TokenType::UNKNOWN);
  REQUIRE(std::empty(value));
  REQUIRE(typeStr == "{ type: UNKNOWN, value: }");
}
