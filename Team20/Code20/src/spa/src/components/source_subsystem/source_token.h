#ifndef SOURCE_TOKEN_H
#define SOURCE_TOKEN_H

#include <string>

enum class SourceTokenType {
  UNKNOWN,
  NEW_LINE,
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
  SourceTokenType type;
  std::string value;

 public:
  SourceToken(SourceTokenType type, std::string value);
  SourceTokenType GetType();
  std::string GetTypeStr();
  std::string GetValue();
};

#endif //SOURCE_TOKEN_H
