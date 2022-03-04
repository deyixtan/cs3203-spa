#ifndef PQL_TOKEN_H
#define PQL_TOKEN_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class PqlTokenType {
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
  EQUAL,
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
  WITH,
  AND,
  BOOLEAN,

  FOLLOWS,
  FOLLOWS_T,
  PARENT,
  PARENT_T,
  USES,
  MODIFIES,
  NEXT,
  NEXT_T,
  CALLS,
  CALLS_T,
  AFFECTS,
  AFFECTS_T,
  PATTERN,

  EXPR,
  IDENT_WITH_QUOTES,
  SUB_EXPRESSION,

  ATTRIBUTE,
  TUPLE
};

class PqlToken {
 public:
  PqlTokenType type;
  std::string value;
  bool operator==(const PqlToken &other) const {
    return type == other.type && value == other.value;
  }
  PqlToken();
  PqlToken(PqlTokenType specified_token_type, std::string specified_value);
};

extern std::unordered_map<std::string, PqlTokenType> string_token_map;
extern std::unordered_set<PqlTokenType> rel_ref;
extern std::unordered_set<PqlTokenType> design_entities;
extern std::unordered_set<PqlTokenType> expression_spec;
extern std::unordered_set<PqlTokenType> ent_ref;
extern std::unordered_set<PqlTokenType> stmt_ref;
extern std::unordered_set<PqlTokenType> line_no_set;
extern std::unordered_set<PqlTokenType> var_set;
extern std::unordered_set<PqlTokenType> allowed_synonyms;
extern std::unordered_set<PqlTokenType> result_cl_excluding_synonym;
extern std::unordered_set<PqlTokenType> ent_ref_excluding_synonym;
extern std::unordered_set<PqlTokenType> stmt_ref_excluding_synonym;

#endif