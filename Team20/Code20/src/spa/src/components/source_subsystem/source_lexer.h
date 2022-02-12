#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_LEXER_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_LEXER_H_

#include <regex>
#include <string>
#include <vector>

#include "source_token.h"

namespace source {

class SourceLexer {
 private:
  int m_cursor;
  std::string m_simple_source;
  std::vector<std::pair<std::regex, TokenType>> m_lexer_specs;

 private:
  void ConstructSpecs();
  bool HasMoreTokens();
  std::shared_ptr<SourceToken> GetNextToken();
  void TryTranslateToKeywordToken(std::shared_ptr<SourceToken> &token_ptr);

 public:
  SourceLexer(std::string simple_source);
  void Tokenize(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr);
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_LEXER_H_
