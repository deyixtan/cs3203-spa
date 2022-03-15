#include "utils.h"

std::string Utils::RemoveSpace(const std::string &token) {
  std::string token_no_space(token);
  token_no_space.erase(std::remove_if(token_no_space.begin(), token_no_space.end(), isspace), token_no_space.end());
  return token_no_space;
}

bool Utils::ContainsDot(const std::string &token) {
  for (auto c : token) {
    if (c == '.') {
      return true;
    }
  }
  return false;
}