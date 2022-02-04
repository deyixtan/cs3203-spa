#ifndef PQL_TOKEN_H
#define PQL_TOKEN_H

#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType {
  SYNONYM,
  VARNAME,
  PROG_LINE,
  SEMICOLON,
  COMMA,
  OPEN_PARENTHESIS,
  CLOSED_PARENTHESIS,
  UNDERSCORE,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  DOUBLE_QUOTE,
  NUMBER,
  STRING,
  SUB_EXPRESSION,

  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ELSE,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,

  SELECT_CL,
  SELECT,
  DECLARATION,
  DESIGN_ENTITY,
  SUCHTHAT_CL,
  SUCH,
  THAT,

  RELREF,
  FOLLOWS,
  FOLLOWS_T,
  PARENT,
  PARENT_T,
  USES,
  USES_S,
  USES_P,
  MODIFIES,
  MODIFIES_S,
  MODIFIES_P,

  ENT_REF,
  PATTERN_CL,
  PATTERN,
  EXPRESSION_SPEC,
  EXPR,
  TERM,
  FACTOR,
  VAR_NAME,
  CONST_VALUE,
};

class PqlToken {
 public:
  TokenType type;
  std::string value;
  bool operator==(const PqlToken &other) const {
    return type == other.type && value == other.value;
  }
  PqlToken(TokenType specified_token_type, std::string specified_value = "") {
    this->type = specified_token_type;
    this->value = specified_value;
  }
};

extern std::unordered_map<std::string, TokenType> string_token_map;

#endif