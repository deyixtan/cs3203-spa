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
  bool IsStringToken(const std::string &token);
  bool IsSubExpressionToken(const std::string &token);
  bool IsIdent(const std::string &s);
  std::string RemoveSpace(const std::string &s);
  std::vector<std::string> Split(std::string s);
};

#endif
