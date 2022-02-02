#ifndef SPA_SOURCECONTROLLER_H
#define SPA_SOURCECONTROLLER_H

#include <string>

#include "utils/FileUtil.h"

class SourceController {
public:
    SourceController();
    std::string RetrieveFileContent(std::string filePath);
};


#endif //SPA_SOURCECONTROLLER_H
