#include "source_controller.h"

SourceController::SourceController() {}

void SourceController::Tokenize(std::string program_source, std::vector<SourceToken *> &tokens_ptr) {
  SourceLexer lexer = SourceLexer(program_source);
  lexer.Tokenize(tokens_ptr);
}

std::shared_ptr<ProgramNode> SourceController::ParseTokenStream(std::vector<SourceToken *> &tokens_ptr) {
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
