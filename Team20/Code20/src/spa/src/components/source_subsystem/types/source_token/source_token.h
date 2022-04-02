#ifndef SOURCE_TOKEN_H
#define SOURCE_TOKEN_H

#include <string>

namespace source {

enum class TokenType {
  UNKNOWN, WHITE_SPACE, NEW_LINE, TAB,
  INTEGER, NAME,
  OPENED_BRACES, CLOSED_BRACES, OPENED_PARENTHESIS, CLOSED_PARENTHESIS,
  NOT, AND, OR,
  IS_EQUAL, IS_NOT_EQUAL, IS_GREATER_EQUAL, IS_LESSER_EQUAL, IS_GREATER, IS_LESSER,
  ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, MODULUS, EQUAL, SEMI_COLON,
  // Specialised statement tokens
  PROCEDURE, READ, PRINT, CALL, WHILE, IF, THEN, ELSE
};

class SourceToken {
 private:
  TokenType m_type;
  std::string m_value;

 public:
  SourceToken(TokenType type, std::string value);
  [[nodiscard]] TokenType GetType();
  [[nodiscard]] std::string GetValue();
};

}

#endif //SOURCE_TOKEN_H
