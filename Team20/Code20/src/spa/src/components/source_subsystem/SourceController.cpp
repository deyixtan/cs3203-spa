#include "SourceController.h"

SourceController::SourceController() {}

void SourceController::Tokenize(std::string program_source, std::vector<SourceToken*> &tokens_ptr) {
  SourceLexer lexer = SourceLexer(program_source);
  lexer.Tokenize(tokens_ptr);
}

std::string SourceController::RetrieveFileContent(std::string file_path) {
    return FileUtil::ReadFileContent(file_path);
}
