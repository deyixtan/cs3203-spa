#ifndef SOURCE_LEXER_H
#define SOURCE_LEXER_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class SourceToken;

class SourceLexer {
 private:
  int m_cursor;
  std::string m_source;

 private:
  [[nodiscard]] bool HasMoreTokens();
  [[nodiscard]] TokenPtr GetNextToken();
  void TwoCharsTokenHandler(TokenPtr &token, char curr_char, char next_char);
  void OneCharTokenHandler(TokenPtr &token, char curr_char);
  void MultipleCharsTokenHandler(TokenPtr &token, char curr_char);
  void IntegerTokenHandler(TokenPtr &token, char curr_char);
  void NameTokenHandler(TokenPtr &token, char curr_char);
  static void SanitizeTokenStream(TokenStream &token_stream);
  static void EncodeTokenStream(TokenStream &token_stream);
  static void EncodeTokenHandler(TokenPtr &token, TokenPtr &next_token);

 public:
  explicit SourceLexer(std::string source);
  void Tokenize(TokenStream &token_stream);
};

}

#endif //SOURCE_LEXER_H
