#include "source_lexer.h"

SourceLexer::SourceLexer(std::string program_source) {
  this->program_source = program_source;
}

std::vector<SourceToken> SourceLexer::Tokenize() {
  std::vector<SourceToken> tokens;
  // TODO: add tokenize implementation, populate "tokens" vector with results
  return tokens;
}
