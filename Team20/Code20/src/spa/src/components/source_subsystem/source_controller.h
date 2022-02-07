#ifndef SOURCE_CONTROLLER_H
#define SOURCE_CONTROLLER_H

#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/source_parser.h"
#include "utils/file_util.h"

class SourceController {
 public:
  SourceController();
  void Tokenize(std::string program_source, std::vector<SourceToken *> &tokens_ptr);
  std::shared_ptr<ProgramNode> ParseTokenStream(std::vector<SourceToken *> &tokens_ptr);
  std::string RetrieveFileContent(std::string file_path);
};

#endif //SOURCE_CONTROLLER_H
