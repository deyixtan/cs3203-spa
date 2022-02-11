#ifndef SOURCE_CONTROLLER_H
#define SOURCE_CONTROLLER_H

#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/source_parser.h"
#include "components/pkb/design_extractor/design_extractor.h"
#include "utils/file_util.h"

namespace source {

class SourceController {
 public:
  SourceController();
  void Tokenize(std::string program_source, std::vector<SourceToken *> &tokens_ptr);
  std::shared_ptr<ProgramNode> ParseTokenStream(std::vector<SourceToken *> &tokens_ptr);
  std::string RetrieveFileContent(std::string file_path);
  void PopulatePKB(PKB *pkb, std::shared_ptr<ProgramNode> ast);
};

}

#endif //SOURCE_CONTROLLER_H
