#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_LEXER_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_LEXER_H_

#include <regex>
#include <string>
#include <vector>

#include "../types/source_token/source_token.h"
#include "components/source_subsystem/exceptions/end_of_stream.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"

namespace source {

class SourceLexer {
 private:
  int m_cursor;
  std::string m_simple_source;
  std::vector<std::pair<std::regex, TokenType>> m_lexer_specs;

 private:
  void ConstructLexerSpecs();
  bool HasMoreTokens();
  [[nodiscard]] std::shared_ptr<SourceToken> GetNextToken();
  void RemoveWhiteSpaceTokens(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr);
  void TranslateKeywordTokens(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr);

 public:
  explicit SourceLexer(std::string simple_source);
  void Tokenize(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr);
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_LEXER_H_
