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
    } else if (IsValidString(token)) {
      if (IsEntRef(token)) {
        tokens.push_back(PqlToken{TokenType::IDENT_WITH_QUOTES, RemoveSpace(token)});
      } else {
        tokens.push_back(PqlToken{TokenType::EXPR, RemoveSpace(token)});
      }
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

bool PqlLexer::IsEntRef(const std::string &s) {
  if (!isalpha(s[1])) {
    return false;
  }

  for (int i = 2; i < s.size() - 1; i++) {
    const char c = s[i];
    if (!isalpha(c) || !isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool PqlLexer::IsSubExpressionToken(const std::string &token) {
  std::string s = token.substr(1, token.size() - 2);
  bool is_valid_string = IsValidString(s);
  return token.size() >= 4 && token.at(0) == '_' && token.back() == '_' && is_valid_string;
}

std::string PqlLexer::RemoveSpace(const std::string &token) {
  std::string token_no_space(token);
  token_no_space.erase(std::remove_if(token_no_space.begin(), token_no_space.end(), isspace), token_no_space.end());
  return token_no_space;
}

std::vector<std::string> PqlLexer::BreakString(const std::string &s) {
  std::unordered_set<char> mathematical_signs = {'*', '/', '+', '-', '%', '(', ')'};
  std::vector<std::string> raw_tokens;
  std::string single_raw_token;
  for (const char c : s) {
    if (isspace(c)) {
      if (!single_raw_token.empty()) {
        raw_tokens.push_back(single_raw_token);
        single_raw_token.clear();
      }
    } else if (mathematical_signs.count(c)) {
      if (!single_raw_token.empty()) {
        raw_tokens.push_back(single_raw_token);
        single_raw_token.clear();
      }
      raw_tokens.push_back(std::string(1, c));
    } else {
      single_raw_token.push_back(c);
    }
  }
  if (!single_raw_token.empty()) {
    raw_tokens.push_back(single_raw_token);
    single_raw_token.clear();
  }
  return raw_tokens;
}

bool PqlLexer::IsValidString(const std::string &s) {
  // 1. cannot end with mathematical signs
  // 2. cannot have 2 consecutive '*', '/', '+', '-', '%', '(', ')', 2 synonyms/integers
  // 3. cannot start with mathematical signs
  // 4. '(' cannot be followed by mathematical signs except for '('
  // 5. mathematical signs except for ')' cannot be followed by ')'
  // 6. '(' must be followed* by ')'

  if (s.at(0) != '"' || s.back() != '"') {
    return false;
  }
  // trim the double quote
  std::string expression = s.substr(1, s.size() - 2);
  // trim left
  while (!isspace(expression[0])) {
    expression = expression.substr(1, expression.size() - 1);
  }
  std::unordered_set<char> mathematical_signs = {'*', '/', '+', '-', '%'};
  std::vector<std::string> string_tokens = BreakString(expression);

  // To be continued

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
        if (!single_raw_token.empty()) {
          raw_tokens.push_back(single_raw_token);
          single_raw_token.clear();
        }
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
          if (!single_raw_token.empty()) {
            raw_tokens.push_back(single_raw_token);
            single_raw_token.clear();
          }
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
          if (!single_raw_token.empty()) {
            raw_tokens.push_back(single_raw_token);
            single_raw_token.clear();
          }
          raw_tokens.push_back(std::string(1, c));
        }
      }
    } else {
      single_raw_token.push_back(c);
    }
  }
  if (!single_raw_token.empty()) {
    raw_tokens.push_back(single_raw_token);
    single_raw_token.clear();
  }
  return raw_tokens;
}



