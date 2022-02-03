#include "SourceController.h"

SourceController::SourceController() {}

std::vector<SourceToken> SourceController::Tokenize(std::string program_source) {
  SourceLexer lexer = SourceLexer(program_source);
  return lexer.Tokenize();
}

std::string SourceController::RetrieveFileContent(std::string filePath) {
    return FileUtil::ReadFileContent(filePath);
}
