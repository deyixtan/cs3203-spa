#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_TOKEN_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_TOKEN_H_

#include <string>

namespace source {

enum class TokenType {
  WHITE_SPACE, NEW_LINE, INTEGER, NAME,
  OPENED_BRACES, CLOSED_BRACES, OPENED_PARENTHESIS, CLOSED_PARENTHESIS,
  NOT, AND, OR,
  IS_GREATER, IS_GREATER_EQUAL, IS_LESSER, IS_LESSER_EQUAL, IS_EQUAL, IS_NOT_EQUAL,
  ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, MODULUS,
  EQUAL, SEMI_COLON,
  READ, PRINT, WHILE, IF, THEN, ELSE, CALL, PROCEDURE,
  UNKNOWN
};

class SourceToken {
 private:
  TokenType m_type;
  std::string m_value;

 public:
  SourceToken(TokenType type, std::string value);
  [[nodiscard]] TokenType GetType();
  [[nodiscard]] std::string GetValue();
  [[nodiscard]] std::string ToString();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_TOKEN_H_
