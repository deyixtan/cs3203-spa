#ifndef SOURCE_LEXER_H
#define SOURCE_LEXER_H

#include <regex>
#include <string>
#include <vector>

#include "components/source_subsystem/source_token.h"

class SourceLexer {
 private:
  std::string program_source;
  int cursor;
  std::vector<std::pair<std::regex, TokenType>> tokenize_specs;

 public:
  SourceLexer(std::string program_source);
  void ConstructSpecs();
  bool IsEOF();
  bool HasMoreTokens();
  SourceToken *GetNextToken();
  void TryTranslateToKeywordToken(SourceToken *&token_ptr);
  void Tokenize(std::vector<SourceToken *> &tokens_ptr);
};

#endif //SOURCE_LEXER_H
