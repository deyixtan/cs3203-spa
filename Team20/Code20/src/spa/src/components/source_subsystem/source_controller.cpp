#include "source_controller.h"

namespace source {

SourceController::SourceController() {}

void SourceController::Tokenize(std::string program_source, std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  SourceLexer lexer = SourceLexer(program_source);
  try {
    lexer.Tokenize(tokens_ptr);
  } catch (const UnexpectedTokenException &unexpected_token) {
    std::cout << unexpected_token.what() << std::endl;
    tokens_ptr.clear();
  }
}

std::shared_ptr<ProgramNode> SourceController::ParseTokenStream(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  SourceParser parser = SourceParser(tokens_ptr);
  std::shared_ptr<ProgramNode> program_node;
  try {
    program_node = parser.ParseProgram();
  } catch (const std::exception &exception) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << exception.what() << std::endl;
  }
  return program_node;
}

std::string SourceController::RetrieveFileContent(std::string file_path) {
  return FileUtil::ReadFileContent(file_path);
}

void SourceController::PopulatePKB(PKB *pkb, std::shared_ptr<ProgramNode> ast) {
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(pkb);
  DesignExtractor design_extractor = DesignExtractor(pkb_client);
  CfgBuilder cfg_builder = CfgBuilder(pkb_client);

  design_extractor.IterateAstAndPopulatePkb(ast);
  cfg_builder.IterateAstAndPopulatePkb(ast);
}

}
