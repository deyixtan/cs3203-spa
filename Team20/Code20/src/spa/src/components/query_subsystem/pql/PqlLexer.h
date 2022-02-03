#pragma once

#include <string>
#include <vector>

#include "PqlToken.h"
using namespace std;

class PqlLexer {
public:
    PqlLexer(string query);
    vector<PqlToken> lex();
private:
    string query;
    bool isAlphaNumeric(const string &s);
    bool startsWithAlphabet(const string &s);
    bool isDigits(const string &s);
    bool isStringToken(const string &token);
    bool isIdent(const string &s);
    vector<string> split(string s);
};
