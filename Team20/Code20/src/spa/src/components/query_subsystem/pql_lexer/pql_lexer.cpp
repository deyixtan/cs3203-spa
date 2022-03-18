#include "pql_lexer.h"
#include "pql_token.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <stack>
#include <sstream>
#include "../utils.h"

// public
PqlLexer::PqlLexer(std::string query) { this->query = query; }

std::vector<PqlToken> PqlLexer::Lex() {
  std::vector<std::string> raw_tokens = Split(query);
  std::vector<PqlToken> tokens;
  for (const auto token : raw_tokens) {
    if (token.size() == 0) {
      continue;
    }
    std::string getTuple = GetValidTuple(token);
    if (string_token_map.find(token) != string_token_map.end()) {
      tokens.push_back(PqlToken{string_token_map[token], token});
    } else if (IsIdent(token)) {
      tokens.push_back(PqlToken{PqlTokenType::SYNONYM, token});
    } else if (IsDigits(token)) {
      tokens.push_back(PqlToken{PqlTokenType::NUMBER, token});
    } else if (IsValidAttribute(token)) {
      tokens.push_back(PqlToken{PqlTokenType::ATTRIBUTE, token});
    } else if (getTuple.length() > 0) {
      tokens.push_back(PqlToken{PqlTokenType::TUPLE, getTuple});
    } else if (IsValidString(token)) {
      if (IsEntRef(token)) {
        std::string no_space_token = Utils::RemoveSpace(token);
        tokens.push_back(PqlToken{PqlTokenType::IDENT_WITH_QUOTES, Utils::TrimQuotes(no_space_token)});
      } else {
        std::string no_space_token = Utils::RemoveSpace(token);
        tokens.push_back(PqlToken{PqlTokenType::EXPR, Utils::TrimQuotes(no_space_token)});
      }
    } else if (IsSubExpressionToken(token)) {
      std::string no_space_token = Utils::RemoveSpace(token);
      tokens.push_back(PqlToken{PqlTokenType::SUB_EXPRESSION, Utils::TrimUnderscoreAndQuotes(no_space_token)});
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
  if(s.length() > 1 && s[0] == '0') {
    return false;
  }
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
    if (!isalpha(c) && !isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool PqlLexer::IsSubExpressionToken(const std::string &token) {
  if (token.size() <= 4) {
    return false;
  }
  std::string s = token.substr(1, token.size() - 2);
  bool is_valid_string = IsValidString(s);
  return token.at(0) == '_' && token.back() == '_' && is_valid_string;
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

bool PqlLexer::IsValidSynonym(const std::string &s) {
  std::unordered_set<char> mathematical_signs = {'*', '/', '+', '-', '%'};
  for (const auto c : s) {
    if (!mathematical_signs.count(c) && !isalpha(c) && !isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool PqlLexer::IsValidAttribute(const std::string &s) {
  std::unordered_set<std::string> attr_name = {"procName", "varName", "value", "stmt#"};
  size_t dot = s.find('.');
  std::string attr = s.substr(dot + 1, s.length() - dot - 1);
  if (!attr_name.count(attr)) {
    return false;
  }
  return true;
}

std::string PqlLexer::GetValidTuple(const std::string &s) {
  int len = s.length();
  if(!(s[0] == '<' && s[len - 1] == '>')) {
    return "";
  }
  std::string temp = s.substr(1, s.length() - 2);
  std::string res = "<";
  while (temp.length()> 0) {
    size_t comma = temp.find(',');
    if (comma != std::string::npos && comma > 0) {
      std::string str = Trim(temp.substr(0, comma));
      if (IsIdent(str) || IsValidAttribute(str)) {
        res += str;
        res += ",";
      } else {
        return "";
      }
      temp = temp.substr(comma + 1, temp.length() - comma - 1);
    }
    else if (IsIdent(Trim(temp)) || IsValidAttribute(Trim(temp))) {
      res += Trim(temp);
      return res + '>';
    }
    else {
      return "";
    }
  }
  return "";
}

// Trim leading and trailing whitespaces
std::string PqlLexer::Trim(const std::string &s) {
  std::string ws = " \n\r\t\f\v";
  size_t start = s.find_first_not_of(ws);
  std::string ltrim = (start == std::string::npos) ? "" : s.substr(start);
  size_t end = ltrim.find_last_not_of(ws);
  return (end == std::string::npos) ? "" : ltrim.substr(0, end + 1);

}

// 1. cannot end with mathematical signs
// 2. cannot have 2 consecutive '*', '/', '+', '-', '%', '(', ')', 2 synonyms/integers
// 3. cannot start with mathematical signs
// 4. '(' cannot be followed by mathematical signs except for '('
// 5. mathematical signs except for ')' cannot be followed by ')'
// 6. '(' must be followed* by ')'
bool PqlLexer::IsValidString(const std::string &s) {
  std::stack<std::string> stack;
  if (s.size() <= 2) {
    return false;
  }
  if (s.at(0) != '"' || s.back() != '"') {
    return false;
  }
  // trim the double quote
  std::string expression = s.substr(1, s.size() - 2);
  std::unordered_set<char> mathematical_signs = {'*', '/', '+', '-', '%', '='};
  std::vector<std::string> string_tokens = BreakString(expression);
  // checks for 1 and 3
  if(mathematical_signs.count(string_tokens[0][0])) {
    return false;
  }
  if(mathematical_signs.count(string_tokens[string_tokens.size() - 1][0])) {
    return false;
  }
  std::string prev = " ";
  bool prev_synonym = false;
  bool prev_number = false;
  for (auto str : string_tokens) {
    // check for strings with length > 1
    if (str.size() > 1) {
      // check that its a valid synonym
      if(!IsValidSynonym(str)) {
        return false;
      }
      if (IsIdent(str) && prev_synonym) {
        return false;
      } else if(IsIdent(str)) {
        prev_synonym = true;
        prev = " ";
      } else {
        if(!IsDigits(str)) {
          return false;
        }
        if (prev_number) {
          return false;
        } else {
          prev_number = true;
          prev = " ";
        }
      }
    } else {
      if (str == "(") {
        stack.push("(");
        prev = "(";
        prev_number = false;
        prev_synonym = false;
      } else if (str == ")") {
        if (stack.empty() || stack.top() != "(" || mathematical_signs.count(prev[0])) {
          return false;
        } else {
          stack.pop();
          prev = ")";
        }
        prev_number = false;
        prev_synonym = false;
      } else {
        char curr = str[0];
        // 2 consecutive math signs
        if (mathematical_signs.count(curr)) {
          if (mathematical_signs.count(prev[0]) || prev == "(") {
            return false;
          }
          prev = str;
          prev_number = false;
          prev_synonym = false;
        } else if (isdigit(curr)) {
          if (prev_number) {
            return false;
          }
          prev = str;
          prev_number = true;
          prev_synonym = false;
        } else if (isalpha(curr)) {
          if (prev_synonym) {
            return false;
          }
          prev = str;
          prev_synonym = true;
          prev_number = false;
        }
      }
    }
  }
  return true;
}

std::vector<std::string> PqlLexer::Format(const std::string &s, char delimeter) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string token;

  while (getline(ss, token, delimeter)) {
    result.push_back(token);
  }

  return result;
}

std::vector<std::string> PqlLexer::Split(std::string s) {
  std::vector<char> charArr;
  const char delimiter = '^';
  bool isString = false;
  bool isTuple = false;
  for (const char c : s) {
    switch (c) {
      // Check for string literals
      case '<':
        isTuple = !isTuple;
        break;
      case '>':
        isTuple = !isTuple;
        break;
      case '"':
        isString = !isString;
        break;
        // Replace spaces w delimiter
      case ' ':
      case '\n':
      case '\t':
        if (!isString && !isTuple) {
          charArr.push_back(delimiter);
          continue;
        }
        break;
        // Characters that will not appear in string
      case '=':
      case ';':
      case ',':
        if(!isTuple) {
          charArr.push_back(delimiter);
        }
        break;
      case ')':
      case '(':
        if (!isString) {
          charArr.push_back(delimiter);
        }
        break;
      default:
        break;
    }
    charArr.push_back(c);
    switch (c) {
      case ';':
      case '=':
      case ',':
        if(!isTuple) {
          charArr.push_back(delimiter);
        }
        break;
      case '(':
      case ')':
        if (!isString) {
          charArr.push_back(delimiter);
        }
        break;
      default:
        break;
    }
  }
  return Format(std::string(charArr.begin(), charArr.end()), delimiter);
}
