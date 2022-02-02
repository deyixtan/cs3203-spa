#include "SourceController.h"

SourceController::SourceController() {}

std::string SourceController::RetrieveFileContent(std::string filePath) {
    return FileUtil::ReadFileContent(filePath);
}
