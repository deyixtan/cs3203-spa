#include "pql_token.h"

std::unordered_map<std::string, TokenType> string_token_map = {

    {";", TokenType::SEMICOLON},
    {",", TokenType::COMMA},
    {"(", TokenType::OPEN_PARENTHESIS},
    {")", TokenType::CLOSED_PARENTHESIS},
    {"_", TokenType::UNDERSCORE},
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MULTIPLY},
    {"/", TokenType::DIVIDE},
    {"\"", TokenType::DOUBLE_QUOTE},

    {"stmt", TokenType::STMT},
    {"read", TokenType::READ},
    {"print", TokenType::PRINT},
    {"call", TokenType::CALL},
    {"while", TokenType::WHILE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"assign", TokenType::ASSIGN},
    {"variable", TokenType::VARIABLE},
    {"constant", TokenType::CONSTANT},
    {"procedure", TokenType::PROCEDURE},
    {"assign", TokenType::ASSIGN},

    {"Select", TokenType::SELECT},
    {"such", TokenType::SUCH},
    {"that", TokenType::THAT},

    {"Follows", TokenType::FOLLOWS},
    {"Follows*", TokenType::FOLLOWS_T},
    {"Parent", TokenType::PARENT},
    {"Parent*", TokenType::PARENT_T},
    {"Uses", TokenType::USES},
    {"Modifies", TokenType::MODIFIES},
    {"pattern", TokenType::PATTERN}
};

std::unordered_set<TokenType> rel_ref = {
    TokenType::FOLLOWS,
    TokenType::FOLLOWS_T,
    TokenType::PARENT,
    TokenType::PARENT_T,
    TokenType::USES,
    TokenType::MODIFIES
};

std::unordered_set<TokenType> design_entities = {
    TokenType::STMT,
    TokenType::READ,
    TokenType::PRINT,
    TokenType::CALL,
    TokenType::WHILE,
    TokenType::IF,
    TokenType::ASSIGN,
    TokenType::VARIABLE,
    TokenType::CONSTANT,
    TokenType::PROCEDURE
};

std::unordered_set<TokenType> expression_spec = {
    TokenType::EXPR,
    TokenType::SUB_EXPRESSION,
    TokenType::UNDERSCORE
};

std::unordered_set<TokenType> ent_ref = {
    TokenType::SYNONYM,
    TokenType::UNDERSCORE,
    TokenType::IDENT_WITH_QUOTES,
};

std::unordered_set<TokenType> stmt_ref = {
    TokenType::SYNONYM,
    TokenType::UNDERSCORE,
    TokenType::NUMBER,
};

