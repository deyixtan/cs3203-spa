#ifndef SOURCE_TOKEN_H
#define SOURCE_TOKEN_H

#include <string>

enum class TokenType {
  UNKNOWN,
  WHITE_SPACE,
  DIGIT,
  NAME,
  OPENED_BRACES,
  CLOSED_BRACES,
  OPENED_PARENTHESIS,
  CLOSED_PARENTHESIS,
  AND,
  OR,
  IS_GREATER_EQUAL,
  IS_GREATER,
  IS_LESSER_EQUAL,
  IS_LESSER,
  IS_EQUAL,
  IS_NOT_EQUAL,
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  MODULUS,
  SEMI_COLON,
  EQUAL,
  NOT,
  PROCEDURE,
  READ,
  PRINT,
  WHILE,
  IF,
  THEN,
  ELSE
};

class SourceToken {
 private:
  TokenType type;
  std::string value;

 public:
  SourceToken(TokenType type, std::string value);
  TokenType GetType();
  std::string GetTypeStr();
  std::string GetValue();
};

#endif //SOURCE_TOKEN_H