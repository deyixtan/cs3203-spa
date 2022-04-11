#include "pql_lexer.h"
#include "pql_token.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <stack>
#include <sstream>
#include "../utils.h"
#include "../exceptions/unrecognised_token.h"

using namespace pql;

// public
PqlLexer::PqlLexer(std::string query) : query(query) {
  string_token_map = {
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
      {"BOOLEAN", PqlTokenType::BOOLEAN},

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
}

std::vector<PqlToken> PqlLexer::Lex() {
  std::vector<std::string> raw_tokens = Split(query);
  std::vector<PqlToken> tokens;
  for (int i = 0; i < raw_tokens.size(); i++) {
    std::string token = raw_tokens[i];
    token = Trim(token);
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
      tokens.push_back(PqlToken{PqlTokenType::ATTRIBUTE, Utils::RemoveSpace(token)});
    } else if (getTuple.length() > 0) {
      tokens.push_back(PqlToken{PqlTokenType::TUPLE, Utils::RemoveSpace(getTuple)});
    } else if (IsValidString(token)) {
      std::string no_space_token = Utils::RemoveSpace(token);
      if (IsEntRef(no_space_token)) {
        tokens.push_back(PqlToken{PqlTokenType::IDENT_WITH_QUOTES, Utils::TrimQuotes(no_space_token)});
      } else {
        tokens.push_back(PqlToken{PqlTokenType::EXPR, Utils::TrimQuotes(no_space_token)});
      }
    } else if (IsSubExpressionToken(token)) {
      std::string no_space_token = Utils::RemoveSpace(token);
      tokens.push_back(PqlToken{PqlTokenType::SUB_EXPRESSION, Utils::TrimUnderscoreAndQuotes(no_space_token)});
    } else {
      throw UnrecognisedTokenException();
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
  std::string s = Trim(token.substr(1, token.size() - 2));
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

bool PqlLexer::IsValidAttribute(const std::string &s) {
  std::unordered_set<std::string> attr_name = {"procName", "varName", "value", "stmt#"};
  size_t dot = s.find('.');
  std::string synonym = s.substr(0, dot);
  synonym = Trim(synonym);
  if (!IsIdent(synonym)) {
    return false;
  }

  std::string attr = s.substr(dot + 1, s.length() - dot - 1);
  attr = Trim(attr);

  if (!attr_name.count(attr)) {
    return false;
  }
  return true;
}

std::string PqlLexer::GetValidTuple(const std::string &s) {
  Trim(s);
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

bool PqlLexer::IsValidSynonym(const std::string &s) {
  std::unordered_set<char> mathematical_signs = {'*', '/', '+', '-', '%'};
  for (const auto c : s) {
    if (!mathematical_signs.count(c) && !isalpha(c) && !isdigit(c)) {
      return false;
    }
  }
  return true;
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
      if(!IsValidSynonym(str)) {
        return false;
      }
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
  if(stack.size() > 0) {
    return false;
  }
  return true;
}

std::vector<std::string> PqlLexer::Format(const std::string &s, char delimiter) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string token;

  while (getline(ss, token, delimiter)) {
    result.push_back(token);
  }
  return result;
}

std::vector<std::string> PqlLexer::Split(std::string s) {
  std::vector<char> char_arr;
  const char delimiter = '^';
  bool is_string = false;
  bool is_tuple = false;
  bool is_attribute = false;
  bool is_subExpr = false;
  for (const char c : s) {
    switch (c) {
      case '.':is_attribute = !is_attribute;
        for(int i = char_arr.size() - 1; i >= 0; i--) {
          if(char_arr[i] == delimiter) {
            char_arr.erase(char_arr.begin() + i);
          } else {
            break;
          }
        }
        break;
      case '_':is_subExpr = !is_subExpr;
        break;
      case '<':
        is_tuple = !is_tuple;
        char_arr.push_back(delimiter);
        break;
      case '>':is_tuple = !is_tuple;
        break;
      case '"':is_string = !is_string;
        break;
        // Replace spaces w delimiter
      case ' ':
      case '\n':
      case '\t':
        if (!is_string && !is_tuple && !is_subExpr && !is_attribute) {
          char_arr.push_back(delimiter);
        }
        break;
        // Characters that will not appear in string
      case '=':
      case ';':
      case ',':
        if (!is_tuple) {
          char_arr.push_back(delimiter);
        }
        if(c == ',' && is_subExpr) {
          is_subExpr = !is_subExpr;
        }
        break;
      case ')':
      case '(':
        if (!is_string) {
          char_arr.push_back(delimiter);
          if (is_subExpr and c == ')') {
            is_subExpr = !is_subExpr;
          }
        }
        break;
      default:break;
    }
    if (c != ' ' || is_subExpr || is_string) {
      char_arr.push_back(c);
      if(c != ' ' && c != '.' && is_attribute) {
        is_attribute = !is_attribute;
      }
    }
    switch (c) {
      case ';':
      case '=':
      case ',':
        if (!is_tuple) {
          char_arr.push_back(delimiter);
        }
        break;
      case '(':
      case ')':
        if (!is_string) {
          char_arr.push_back(delimiter);
        }
        break;
      default:break;
    }
  }
  return Format(std::string(char_arr.begin(), char_arr.end()), delimiter);
}
