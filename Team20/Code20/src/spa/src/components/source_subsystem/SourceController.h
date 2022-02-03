#ifndef SPA_SOURCECONTROLLER_H
#define SPA_SOURCECONTROLLER_H

#include "source_lexer.h"

#include "utils/FileUtil.h"

class SourceController {
public:
    SourceController();
    std::vector<SourceToken*> Tokenize(std::string program_source);
    std::string RetrieveFileContent(std::string filePath);
};


#endif //SPA_SOURCECONTROLLER_H
