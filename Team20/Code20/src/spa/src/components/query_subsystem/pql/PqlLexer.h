#pragma once

#include <string>
#include <vector>

#include "PqlToken.h"

class PqlLexer {
public:
    PqlLexer(std::string query);
    std::vector<PqlToken> lex();
private:
    std::string query;
    bool isAlphaNumeric(const std::string &s);
    bool startsWithAlphabet(const std::string &s);
    bool isDigits(const std::string &s);
    bool isStringToken(const std::string &token);
    bool isIdent(const std::string &s);
    std::vector<std::string> split(std::string s);
};
