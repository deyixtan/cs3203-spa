#include <string>
#include <unordered_map>
#include <unordered_set>
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
    USES_S,
    USES_P,
    MODIFIES_S,
    MODIFIES_P,

    PATTERN_CL,
    PATTERN,
    EXPRESSION_SPEC,
    EXPR,
    TERM,
    FACTOR,
    VAR_NAME,
    CONST_VALUE,
};

struct PqlToken {
    TokenType type;
    std::string value;
    bool operator==(const PqlToken &other) const {
        return type == other.type && value == other.value;
    }
    PqlToken(TokenType specifiedTokenType, std::string specifiedValue = "")
            : type{specifiedTokenType}, value{specifiedValue} {}
};
