#ifndef SOURCE_CONTROLLER_H
#define SOURCE_CONTROLLER_H

#include <iostream>

#include "lexer/source_lexer.h"
#include "parser/source_parser.h"
#include "iterator/design_extractor.h"
#include "iterator/cfg_builder.h"
#include "utils/file_util.h"
#include "source_declarations.h"

namespace source {

class SourceController {
 public:
  SourceController();
  void Tokenize(std::string source, TokenStream &token_stream);
  std::shared_ptr<ProgramNode> ParseTokenStream(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr);
  std::string RetrieveFileContent(std::string file_path);
  void PopulatePKB(PKB *pkb, std::shared_ptr<ProgramNode> ast);
};

}

#endif //SOURCE_CONTROLLER_H
