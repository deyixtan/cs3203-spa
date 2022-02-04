#include "file_util.h"

// static method
std::string FileUtil::ReadFileContent(std::string file_path) {
  std::ifstream file;
  std::stringstream buffer;
  std::string content;

  file.open(file_path);
  if (file.fail()) {
    // TODO: throw custom FileNotFoundException?
    return "";
  }

  buffer << file.rdbuf();
  content = buffer.str();
  file.close();

  return content;
}
