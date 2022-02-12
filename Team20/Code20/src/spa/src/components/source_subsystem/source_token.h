#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_TOKEN_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_TOKEN_H_

#include <string>

namespace source {

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
  TokenType m_type;
  std::string m_value;

 public:
  SourceToken(TokenType type, std::string &value);
  [[nodiscard]] TokenType GetType();
  [[nodiscard]] std::string GetValue();
  [[nodiscard]] std::string ToString();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_TOKEN_H_
