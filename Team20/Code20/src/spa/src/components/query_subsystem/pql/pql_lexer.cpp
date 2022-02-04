#include "pql_lexer.h"
#include "pql_token.h"
#include <string>
#include <unordered_set>
#include <vector>

// public
PqlLexer::PqlLexer(std::string query) { this->query = query; }

std::vector<PqlToken> PqlLexer::Lex() {
  std::vector<std::string> raw_tokens = Split(query);
  std::vector<PqlToken> tokens;
  for (const auto token : raw_tokens) {
    if (token.size() == 0) {
      continue;
    }
    if (string_token_map.find(token) != string_token_map.end()) {
      tokens.push_back(PqlToken{string_token_map[token], token});
    } else if (IsStringToken(token)) {
      tokens.push_back(PqlToken{TokenType::STRING, RemoveSpace(token)});
    } else if (IsSubExpressionToken(token)) {
      tokens.push_back(PqlToken{TokenType::SUB_EXPRESSION, RemoveSpace(token)});
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

bool PqlLexer::IsStringToken(const std::string &token) {
  return token.size() >= 2 && token.at(0) == '"' && token.back() == '"';
}

bool PqlLexer::IsSubExpressionToken(const std::string &token) {
  return token.size() >= 4 && token.at(0) == '_' && token.back() == '_';
}

std::string PqlLexer::RemoveSpace(const std::string &token) {
  std::string token_no_space(token);
  token_no_space.erase(std::remove_if(token_no_space.begin(), token_no_space.end(), isspace), token_no_space.end());
  return token_no_space;
}

std::unordered_set<char> stickChar = {
    ';', ',', '(', ')', '"', '+', '*', '/', '%', '-', '_'
};

std::unordered_set<std::string> relationship = {
    "Follows", "Parent"
};

std::vector<std::string> PqlLexer::Split(std::string s) {
  std::vector<std::string> raw_tokens;
  std::string single_raw_token;
  int double_quote_count = 0;
  int underscore_count = 0;
  for (int i = 0; i < s.size(); i++) {
    const char c = s[i];
    if (i == s.size() - 1 && (underscore_count == 1 || double_quote_count == 1)) {
      single_raw_token.push_back(c);
      raw_tokens.push_back(single_raw_token);
      break;
    }
    if (isspace(c)) {
      if (double_quote_count == 1) {
        single_raw_token.push_back(c);
      } else {
        raw_tokens.push_back(single_raw_token);
        single_raw_token.clear();
      }
    } else if (stickChar.count(c)) {
      if (c == '*' && relationship.count(single_raw_token)) {
        single_raw_token.push_back(c);
      } else if (c == '_') {
        if (underscore_count == 0) { // encountering first _
          raw_tokens.push_back(single_raw_token);
          single_raw_token.clear();
          single_raw_token.push_back(c);
          underscore_count += 1;
        } else { // sub_expression complete
          single_raw_token.push_back(c);
          underscore_count = 0;
          raw_tokens.push_back(single_raw_token);
          single_raw_token.clear();
        }
      } else if (c == '"') {
        if (double_quote_count == 0) {
          if (underscore_count == 1) { // (_, "
            underscore_count = 0;
          }
          raw_tokens.push_back(single_raw_token);
          single_raw_token.clear();
          single_raw_token.push_back(c);
          double_quote_count += 1;
        } else {
          if (underscore_count == 1) { // _"x": string complete, sub-expression incomplete
            single_raw_token.push_back(c);
          } else { // "x"
            single_raw_token.push_back(c);
            raw_tokens.push_back(single_raw_token);
            single_raw_token.clear();
          }
          double_quote_count = 0;
        }
      } else {
        if (double_quote_count == 1) {
          single_raw_token.push_back(c);
        } else {
          underscore_count = 0;
          double_quote_count = 0;
          raw_tokens.push_back(single_raw_token);
          raw_tokens.push_back(std::string(1, c));
          single_raw_token.clear();
        }
      }
    } else {
      single_raw_token.push_back(c);
    }
  }
  return raw_tokens;
}



