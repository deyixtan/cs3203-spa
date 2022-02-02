#ifndef SPA_FILEUTIL_H
#define SPA_FILEUTIL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileUtil {
public:
    static std::string ReadFileContent(std::string filePath);
};


#endif //SPA_FILEUTIL_H
