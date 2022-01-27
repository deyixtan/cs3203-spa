#include "PqlToken.h"

std::unordered_map<std::string, TokenType> stringTokenMap = {
        {"progLine", TokenType::PROG_LINE},
        {";", TokenType::SEMICOLON},
        {",", TokenType::COMMA},
        {"(", TokenType::OPEN_PARENTHESIS},
        {")", TokenType::CLOSED_PARENTHESIS},
        {"_", TokenType::UNDERSCORE},

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
        {"Uses", TokenType::USES_S},
        {"Uses", TokenType::USES_P},
        {"Modifies", TokenType::MODIFIES_S},
        {"Modifies", TokenType::MODIFIES_P},

        {"pattern", TokenType::PATTERN},
        {"(", TokenType::OPEN_PARENTHESIS},
        {")", TokenType::CLOSED_PARENTHESIS},
        {"varName", TokenType::VARNAME},
};
