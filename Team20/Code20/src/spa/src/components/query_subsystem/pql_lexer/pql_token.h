#ifndef PQL_TOKEN_H
#define PQL_TOKEN_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class TokenType {
  SYNONYM,
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

  SELECT,
  SUCH,
  THAT,

  FOLLOWS,
  FOLLOWS_T,
  PARENT,
  PARENT_T,
  USES,
  MODIFIES,
  PATTERN,

  EXPR,
  IDENT_WITH_QUOTES,
  SUB_EXPRESSION,
};

class PqlToken {
 public:
  TokenType type;
  std::string value;
  bool operator==(const PqlToken &other) const {
    return type == other.type && value == other.value;
  }
  PqlToken(TokenType specified_token_type, std::string specified_value);
};

extern std::unordered_map<std::string, TokenType> string_token_map;
extern std::unordered_set<TokenType> rel_ref;
extern std::unordered_set<TokenType> design_entities;
extern std::unordered_set<TokenType> expression_spec;
extern std::unordered_set<TokenType> ent_ref;
extern std::unordered_set<TokenType> stmt_ref;

#endif