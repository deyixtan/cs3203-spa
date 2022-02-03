#ifndef SOURCE_LEXER_H
#define SOURCE_LEXER_H

#include <string>
#include <vector>

#include "source_token.h"

class SourceLexer {
 private:
  std::string program_source;

 public:
  SourceLexer(std::string program_source);
  std::vector<SourceToken> Tokenize();
};

#endif //SOURCE_LEXER_H
