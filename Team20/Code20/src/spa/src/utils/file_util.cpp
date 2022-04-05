#include "file_util.h"

std::string FileUtil::ReadFileContent(std::string &file_path) {
  std::ifstream file;
  std::stringstream buffer;
  std::string content;

  file.open(file_path);
  if (file.fail()) {
    throw std::runtime_error("File not found.");
  }

  buffer << file.rdbuf();
  content = buffer.str();
  file.close();

  return content;
}
