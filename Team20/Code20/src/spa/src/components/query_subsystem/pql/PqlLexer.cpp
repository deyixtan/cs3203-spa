#include "PqlLexer.h"
#include "PqlToken.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

PqlLexer::PqlLexer(string query) { this->query = query; }

// public
vector<PqlToken> PqlLexer::lex() {
    vector<string> raw_tokens = split(query);
    vector<PqlToken> tokens;
    for (const auto token : raw_tokens) {
        if (token.size() == 0) {
            continue;
        }
        if (stringTokenMap.find(token) != stringTokenMap.end()) {
            tokens.push_back(PqlToken{stringTokenMap[token], token});
        } else if (isStringToken(token)) {
            tokens.push_back(PqlToken{TokenType::STRING, token});
        } else if (isIdent(token)) {
            tokens.push_back(PqlToken{TokenType::SYNONYM, token});
        } else if (isDigits(token)) {
            tokens.push_back(PqlToken{TokenType::NUMBER, token});
        } else {
            throw "ERROR: Unrecognised token" + token + "\n";
        }
    }

    return tokens;
}

// private
bool PqlLexer::isAlphaNumeric(const string &s) {
    for (const auto c : s) {
        if (!isalnum(c)) {
            return false;
        }
    }
    return true;
}

bool PqlLexer::startsWithAlphabet(const string &s) {
    if (s.empty()) {
        return false;
    }
    return isalpha(s[0]);
}

bool PqlLexer::isDigits(const string &s) {
    for (const auto c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool PqlLexer::isStringToken(const string &token) {
    return token.size() >= 2 && token.at(0) == '"' && token.back() == '"';
}

bool PqlLexer::isIdent(const string &s) {
    return startsWithAlphabet(s) && isAlphaNumeric(s);
}

unordered_set<char> stickChar = {
        ';', ',', '(', ')', '"', '+', '*', '/', '-', '_'
};

vector<string> PqlLexer::split(string s) {
    vector<string> raw_tokens;
    string single_raw_token;
    for (const char c : s) {
        if (isspace(c)) {
            raw_tokens.push_back(single_raw_token);
            single_raw_token.clear();
        } else if (stickChar.count(c)) {
            raw_tokens.push_back(single_raw_token);
            raw_tokens.push_back(string (1, c));
            single_raw_token.clear();
        } else {
            single_raw_token.push_back(c);
        }
    }
    return raw_tokens;
}



