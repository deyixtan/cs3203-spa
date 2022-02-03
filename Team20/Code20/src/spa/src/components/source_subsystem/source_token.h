#ifndef SOURCETOKEN_H
#define SOURCETOKEN_H

#include <string>

enum class TokenType {
  UNKNOWN,
  DIGIT,
  NAME,
  OPENED_BRACES,
  CLOSED_BRACES,
  OPENED_PARENTHESIS,
  CLOSED_PARENTHESIS,
  SEMI_COLON,
  EQUAL,
  NOT,
  AND,
  OR,
  IS_GREATER,
  IS_GREATER_EQUAL,
  IS_LESSER,
  IS_LESSER_EQUAL,
  IS_EQUAL,
  IS_NOT_EQUAL,
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  MODULUS
};

class SourceToken {
 private:
  TokenType type;
  std::string value;

 public:
  SourceToken(TokenType type, std::string value);
  TokenType GetType();
  std::string GetValue();
};

#endif //SOURCETOKEN_H
