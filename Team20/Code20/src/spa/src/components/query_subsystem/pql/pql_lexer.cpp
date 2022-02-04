#include "pql_lexer.h"
#include "pql_token.h"
#include <string>
#include <unordered_set>
#include <vector>

PqlLexer::PqlLexer(std::string query) { this->query = query; }

// public
std::vector<PqlToken> PqlLexer::Lex() {
  std::vector<std::string> raw_tokens = Split(query);
  std::vector<PqlToken> tokens;
  for (const auto token : raw_tokens) {
    if (token.size() == 0) {
      continue;
    }
    if (string_token_map.find(token) != string_token_map.end()) {
      tokens.push_back(PqlToken{string_token_map[token], token});
    } else if (IsIdent(token)) {
      tokens.push_back(PqlToken{TokenType::SYNONYM, token});
    } else if (IsDigits(token)) {
      tokens.push_back(PqlToken{TokenType::NUMBER, token});
    } else {
      throw "ERROR: Unrecognised token " + token + "\n";
    }
  }

  return tokens;
}

// private
bool PqlLexer::IsAlphaNumeric(const std::string &s) {
  for (const auto c : s) {
    if (!isalnum(c)) {
      return false;
    }
  }
  return true;
}

bool PqlLexer::StartsWithAlphabet(const std::string &s) {
  if (s.empty()) {
    return false;
  }
  return isalpha(s[0]);
}

bool PqlLexer::IsDigits(const std::string &s) {
  for (const auto c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool PqlLexer::IsIdent(const std::string &s) {
  return StartsWithAlphabet(s) && IsAlphaNumeric(s);
}

std::unordered_set<char> stickChar = {
    ';', ',', '(', ')', '"', '+', '*', '/', '-', '_'
};

std::unordered_set<std::string> relationship = {
    "Follows", "Parent"
};

std::vector<std::string> PqlLexer::Split(std::string s) {
  std::vector<std::string> raw_tokens;
  std::string single_raw_token;
  for (const char c : s) {
    if (isspace(c)) {
      raw_tokens.push_back(single_raw_token);
      single_raw_token.clear();
    } else if (stickChar.count(c)) {
      if (c == '*' && relationship.count(single_raw_token)) {
        single_raw_token.push_back(c);
      } else {
        raw_tokens.push_back(single_raw_token);
        raw_tokens.push_back(std::string(1, c));
        single_raw_token.clear();
      }
    } else {
      single_raw_token.push_back(c);
    }
  }
  return raw_tokens;
}



