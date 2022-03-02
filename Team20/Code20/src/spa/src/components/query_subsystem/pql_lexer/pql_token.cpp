#include "pql_token.h"

PqlToken::PqlToken(){};
PqlToken::PqlToken(PqlTokenType specified_token_type, std::string specified_value) {
  this->type = specified_token_type;
  this->value = specified_value;
}

std::unordered_map<std::string, PqlTokenType> string_token_map = {

    {";", PqlTokenType::SEMICOLON},
    {",", PqlTokenType::COMMA},
    {"(", PqlTokenType::OPEN_PARENTHESIS},
    {")", PqlTokenType::CLOSED_PARENTHESIS},
    {"_", PqlTokenType::UNDERSCORE},
    {"+", PqlTokenType::PLUS},
    {"-", PqlTokenType::MINUS},
    {"*", PqlTokenType::MULTIPLY},
    {"/", PqlTokenType::DIVIDE},
    {"=", PqlTokenType::EQUAL},
    {"\"", PqlTokenType::DOUBLE_QUOTE},

    {"stmt", PqlTokenType::STMT},
    {"read", PqlTokenType::READ},
    {"print", PqlTokenType::PRINT},
    {"call", PqlTokenType::CALL},
    {"while", PqlTokenType::WHILE},
    {"if", PqlTokenType::IF},
    {"else", PqlTokenType::ELSE},
    {"assign", PqlTokenType::ASSIGN},
    {"variable", PqlTokenType::VARIABLE},
    {"constant", PqlTokenType::CONSTANT},
    {"procedure", PqlTokenType::PROCEDURE},

    {"Select", PqlTokenType::SELECT},
    {"such", PqlTokenType::SUCH},
    {"that", PqlTokenType::THAT},
    {"with", PqlTokenType::WITH},
    {"and", PqlTokenType::AND},

    {"Follows", PqlTokenType::FOLLOWS},
    {"Follows*", PqlTokenType::FOLLOWS_T},
    {"Parent", PqlTokenType::PARENT},
    {"Parent*", PqlTokenType::PARENT_T},
    {"Uses", PqlTokenType::USES},
    {"Modifies", PqlTokenType::MODIFIES},
    {"Next", PqlTokenType::NEXT},
    {"Next*", PqlTokenType::NEXT_T},
    {"Calls", PqlTokenType::CALLS},
    {"Calls*", PqlTokenType::CALLS_T},
    {"Affects", PqlTokenType::AFFECTS},
    {"Affects*", PqlTokenType::AFFECTS_T},
    {"pattern", PqlTokenType::PATTERN}
};

std::unordered_set<PqlTokenType> rel_ref = {
    PqlTokenType::FOLLOWS,
    PqlTokenType::FOLLOWS_T,
    PqlTokenType::PARENT,
    PqlTokenType::PARENT_T,
    PqlTokenType::USES,
    PqlTokenType::MODIFIES
};

std::unordered_set<PqlTokenType> design_entities = {
    PqlTokenType::STMT,
    PqlTokenType::READ,
    PqlTokenType::PRINT,
    PqlTokenType::CALL,
    PqlTokenType::WHILE,
    PqlTokenType::IF,
    PqlTokenType::ASSIGN,
    PqlTokenType::VARIABLE,
    PqlTokenType::CONSTANT,
    PqlTokenType::PROCEDURE
};

std::unordered_set<PqlTokenType> expression_spec = {
    PqlTokenType::EXPR,
    PqlTokenType::SUB_EXPRESSION,
    PqlTokenType::UNDERSCORE
};

std::unordered_set<PqlTokenType> ent_ref = {
    PqlTokenType::SYNONYM,
    PqlTokenType::UNDERSCORE,
    PqlTokenType::IDENT_WITH_QUOTES,
};

std::unordered_set<PqlTokenType> stmt_ref = {
    PqlTokenType::SYNONYM,
    PqlTokenType::UNDERSCORE,
    PqlTokenType::NUMBER,
};

std::unordered_set<PqlTokenType> line_no_set = {
    PqlTokenType::STMT,
    PqlTokenType::READ,
    PqlTokenType::PRINT,
    PqlTokenType::WHILE,
    PqlTokenType::IF,
    PqlTokenType::ASSIGN,
    PqlTokenType::CALL,
};

std::unordered_set<PqlTokenType> var_set = {
    PqlTokenType::VARIABLE,
    PqlTokenType::CONSTANT,
};

std::unordered_set<PqlTokenType> allowed_synonyms = {
    PqlTokenType::SYNONYM,
    PqlTokenType::FOLLOWS,
    PqlTokenType::PARENT,
    PqlTokenType::USES,
    PqlTokenType::MODIFIES,
    PqlTokenType::NEXT,
    PqlTokenType::CALLS,
    PqlTokenType::AFFECTS,
    PqlTokenType::PATTERN,
    PqlTokenType::STMT,
    PqlTokenType::READ,
    PqlTokenType::PRINT,
    PqlTokenType::CALL,
    PqlTokenType::WHILE,
    PqlTokenType::IF,
    PqlTokenType::ASSIGN,
    PqlTokenType::VARIABLE,
    PqlTokenType::CONSTANT,
    PqlTokenType::PROCEDURE,
    PqlTokenType::SUCH,
    PqlTokenType::THAT,
    PqlTokenType::WITH,
    PqlTokenType::AND,
    PqlTokenType::SELECT,
};

