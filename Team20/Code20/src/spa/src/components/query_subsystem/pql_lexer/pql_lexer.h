#ifndef PQL_LEXER_H
#define PQL_LEXER_H

#include <string>
#include <vector>
#include "pql_token.h"

class PqlLexer {
 public:
  PqlLexer(std::string query);
  std::vector<PqlToken> Lex();

 private:
  std::string query;
  bool IsAlphaNumeric(const std::string &s);
  bool StartsWithAlphabet(const std::string &s);
  bool IsDigits(const std::string &s);
  bool IsEntRef(const std::string &s);
  bool IsSubExpressionToken(const std::string &token);
  bool IsIdent(const std::string &s);
  bool IsValidSynonym(const std::string &s);
  bool IsValidAttribute(const std::string &s);
  std::string GetValidTuple(const std::string &s);
  std::string Trim(const std::string &s);
  std::vector<std::string> BreakString(const std::string &s);
  bool IsValidString(const std::string &s);
  std::vector<std::string> Split(std::string s);
  std::vector<std::string> Format(const std::string &s, char delim);
  std::vector<std::string> Regroup(std::vector<std::string> indiv_token_list);
  int RegroupStarRelationship(int current_index,
                              std::vector<std::string> &regrouped_token_list,
                              const std::string &curr_token) const;
  int RegroupSubExpression(const std::vector<std::string> &indiv_token_list,
                           int max_len,
                           int current_index,
                           std::vector<std::string> &regrouped_token_list) const;
  int RegroupString(const std::vector<std::string> &indiv_token_list,
                    int max_len,
                    int current_index,
                    std::vector<std::string> &regrouped_token_list) const;
  int RegroupTuple(const std::vector<std::string> &indiv_token_list,
                             int max_len,
                             int current_index,
                             std::vector<std::string> &regrouped_token_list) const;

  int RegroupAttribute(const std::vector<std::string> &indiv_token_list,
                       int current_index,
                       std::vector<std::string> &regrouped_token_list,
                       const std::string &curr_token,
                       const std::string &next_token) const;
};

extern std::unordered_set<char> sticky_characters;
extern std::unordered_set<std::string> rel_ref_string;

#endif
