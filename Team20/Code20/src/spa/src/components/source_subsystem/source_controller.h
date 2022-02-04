#ifndef SOURCE_CONTROLLER_H
#define SOURCE_CONTROLLER_H

#include "source_lexer.h"
#include "utils/file_util.h"

class SourceController {
 public:
  SourceController();
  void Tokenize(std::string program_source, std::vector<SourceToken *> &tokens_ptr);
  std::string RetrieveFileContent(std::string file_path);
};

#endif //SOURCE_CONTROLLER_H
