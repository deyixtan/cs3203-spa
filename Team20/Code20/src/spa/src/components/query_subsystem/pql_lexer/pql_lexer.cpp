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
  std::vector<std::string> raw_tokens = Regroup(Split(query));
  std::vector<PqlToken> tokens;
  bool isAttribute = false;
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
      throw std::runtime_error("ERROR: Unrecognised token! \n");
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
      if(!IsIdent(str)) {
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
  for (const char c : s) {
    if (sticky_characters.count(c) != 0) {
      char_arr.push_back(delimiter);
    }
    char_arr.push_back(c);
    if (sticky_characters.count(c) != 0) {
      char_arr.push_back(delimiter);
    }
  }

  std::vector<std::string> reformatted_list = Format(std::string(char_arr.begin(), char_arr.end()), delimiter);
  std::vector<std::string> result_list;
  for (auto token : reformatted_list) {
    if (token != " " && token != "\n" && token != "\t" && !token.empty()) {
      result_list.push_back(token);
    }
  }

  return result_list;
}

std::vector<std::string> PqlLexer::Regroup(std::vector<std::string> indiv_token_list) {
  int max_len = indiv_token_list.size();
  int current_index = 0;
  bool is_end = false;
  std::vector<std::string> regrouped_token_list;

  while (current_index < max_len - 1) {
    std::string curr_token = indiv_token_list[current_index];
    std::string next_token = indiv_token_list[current_index + 1];

    if (curr_token == " " || curr_token == "\n" || curr_token == "\t" || curr_token.empty()) {
      current_index++;
    } else if (rel_ref_string.count(curr_token) != 0 && next_token == "*") { // check for star relationship
      current_index = RegroupStarRelationship(current_index, regrouped_token_list, curr_token);
    } else if (curr_token == "_" && next_token == "\"") { // check for sub_expression
      current_index = RegroupSubExpression(indiv_token_list, max_len, current_index, regrouped_token_list);
    } else if (curr_token == "\"") { // check for string
      current_index = RegroupString(indiv_token_list, max_len, current_index, regrouped_token_list);
      if (current_index >= max_len) {
        is_end = true;
      }
    } else if (curr_token == "<") { // check for tuple
      current_index = RegroupTuple(indiv_token_list, max_len, current_index, regrouped_token_list);
      if (current_index >= max_len) {
        is_end = true;
      }
    } else if (next_token == ".") { // check for attribute
      current_index = RegroupAttribute(indiv_token_list, current_index, regrouped_token_list, curr_token, next_token);
      if (current_index >= max_len) {
        is_end = true;
      }
    } else {
      regrouped_token_list.push_back(curr_token);
      current_index++;
    }
  }

  if (!is_end) {
    regrouped_token_list.push_back(indiv_token_list[max_len - 1]);
  }
  return regrouped_token_list;
}

int PqlLexer::RegroupAttribute(const std::vector<std::string> &indiv_token_list,
                               int current_index,
                               std::vector<std::string> &regrouped_token_list,
                               const std::string &curr_token,
                               const std::string &next_token) const {
  std::string attribute = curr_token + next_token + indiv_token_list[current_index + 2];
  regrouped_token_list.push_back(attribute);
  current_index += 3;
  return current_index;
}

int PqlLexer::RegroupTuple(const std::vector<std::string> &indiv_token_list,
                            int max_len,
                            int current_index,
                            std::vector<std::string> &regrouped_token_list) const {
  int increment = 1;
  std::string tuple = "<";
  while (current_index + increment < max_len) {
    if (indiv_token_list[current_index + increment] == ">") {
      tuple += (indiv_token_list[current_index + increment] + " ");
      break;
    }
    tuple += (indiv_token_list[current_index + increment] + " ");
    increment++;
  }
  if (current_index + increment == max_len) {
    throw "ERROR: Wrong tuple format! \n";
  }
  regrouped_token_list.push_back(tuple);
  current_index += (increment + 1);
  return current_index;
}

int PqlLexer::RegroupString(const std::vector<std::string> &indiv_token_list,
                            int max_len,
                            int current_index,
                            std::vector<std::string> &regrouped_token_list) const {
  int increment = 1;
  std::string string_token = "\"";
  while (current_index + increment < max_len) {
    if (indiv_token_list[current_index + increment] == "\"") {
      string_token += (indiv_token_list[current_index + increment] + " ");
      break;
    }
    string_token += (indiv_token_list[current_index + increment] + " ");
    increment++;
  }
  if (current_index + increment == max_len) {
    throw "ERROR: Wrong string format! \n";
  }
  regrouped_token_list.push_back(string_token);
  current_index += (increment + 1);
  return current_index;
}

int PqlLexer::RegroupSubExpression(const std::vector<std::string> &indiv_token_list,
                                   int max_len,
                                   int current_index,
                                   std::vector<std::string> &regrouped_token_list) const {
  int increment = 1;
  std::string sub_expression = "_";
  while (current_index + increment < max_len - 1) {
    if (indiv_token_list[current_index + increment] == "\"" && indiv_token_list[current_index + increment + 1] == "_") {
      sub_expression += (indiv_token_list[current_index + increment] + indiv_token_list[current_index + increment + 1]);
      break;
    }
    sub_expression += (indiv_token_list[current_index + increment] + " ");
    increment++;
  }
  if (current_index + increment == max_len - 1) {
    throw "ERROR: Wrong sub expression format! \n";
  }
  regrouped_token_list.push_back(sub_expression);
  current_index += (increment + 2);
  return current_index;
}

int PqlLexer::RegroupStarRelationship(int current_index,
                                      std::vector<std::string> &regrouped_token_list,
                                      const std::string &curr_token) const {
  regrouped_token_list.push_back(curr_token + "*");
  current_index += 2;
  return current_index;
}

std::unordered_set<char> sticky_characters = {
  '(', ')', ',', '.',
  ' ', '\n', '\t',
  '+', '-', '*', '/', '%',
  '_', ';',
  '<', '>',
  '\"'
};

std::unordered_set<std::string> rel_ref_string = {
    "Affects",
    "Follows",
    "Parent",
    "Calls",
    "Next"
};
