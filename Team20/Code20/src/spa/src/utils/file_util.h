#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <fstream>
#include <sstream>
#include <string>

class FileUtil {
 public:
  static std::string ReadFileContent(std::string &file_path);
};

#endif //FILE_UTIL_H
