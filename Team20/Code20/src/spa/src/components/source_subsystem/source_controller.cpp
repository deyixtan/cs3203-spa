#include "source_controller.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/lexer/source_lexer.h"
#include "components/source_subsystem/parser/source_parser.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "utils/file_util.h"

namespace source {

String SourceController::RetrieveFileContent(String &file_path) {
  return FileUtil::ReadFileContent(file_path);
}

void SourceController::Tokenize(String &source, TokenStream &token_stream) {
  SourceLexer lexer = SourceLexer(source);
  try {
    lexer.Tokenize(token_stream);
  } catch (const Exception &exception) {
    token_stream.clear();
    std::cout << "ERROR: " << exception.what() << std::endl;
  }
}

ProgramNodePtr SourceController::ParseTokenStream(TokenStream &token_stream) {
  SourceParser parser = SourceParser(token_stream);
  ProgramNodePtr program_node;
  try {
    program_node = parser.ParseProgram();
  } catch (const Exception &exception) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << exception.what() << std::endl;
  }
  return program_node;
}

void SourceController::PopulatePKB(PKB *pkb, ProgramNodePtr &ast) {
  PkbClientPtr pkb_client = std::make_shared<PkbClient>(pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  CfgBuilderPtr cfg_builder = std::make_shared<CfgBuilder>(pkb_client);
  design_extractor->IterateAst(ast);
  cfg_builder->IterateAstAndPopulatePkb(ast);
  cfg_builder->IterateCfgAndPopulatePkb();
}

}
