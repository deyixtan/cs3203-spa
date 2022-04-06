#ifndef PQL_LEXER_H
#define PQL_LEXER_H

#include <string>
#include <vector>
#include "pql_token.h"

using namespace pql;

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
  std::unordered_map<std::string, PqlTokenType> string_token_map;
};

#endif
