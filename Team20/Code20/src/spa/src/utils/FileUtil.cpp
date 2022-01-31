#include "FileUtil.h"

// static method
std::string FileUtil::ReadFileContent(std::string filePath) {
    std::ifstream file;
    std::stringstream buffer;
    std::string content;

    file.open(filePath);
    if (file.fail()) {
        // TODO: throw custom FileNotFoundException?
        return "";
    }

    buffer << file.rdbuf();
    content = buffer.str();
    file.close();

    return content;
}
