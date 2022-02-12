#include "source_controller.h"

namespace source {

SourceController::SourceController() {}

void SourceController::Tokenize(std::string program_source, std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  SourceLexer lexer = SourceLexer(program_source);
  try {
    lexer.Tokenize(tokens_ptr);
  } catch (const UnexpectedTokenException &unexpected_token) {
    tokens_ptr.clear();
  }
}

std::shared_ptr<ProgramNode> SourceController::ParseTokenStream(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  SourceParser parser = SourceParser(tokens_ptr);
  return parser.ParseProgram();
}

std::string SourceController::RetrieveFileContent(std::string file_path) {
  return FileUtil::ReadFileContent(file_path);
}

void SourceController::PopulatePKB(PKB *pkb, std::shared_ptr<ProgramNode> ast) {
  UsageStore usage_store;
  DesignExtractor design_extractor = DesignExtractor(*ast, pkb, usage_store);
  design_extractor.traverse_ast();
}

}
