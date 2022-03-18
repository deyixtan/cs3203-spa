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

std::pair<std::string, AtrriName> Utils::ParseAttributeRef(PqlToken &token) {
  int dot_index = token.value.find('.');
  std::string synonym = token.value.substr(0, dot_index);
  std::string attriName = token.value.substr(dot_index + 1, token.value.size() - (dot_index + 1));
  std::pair<std::string, AtrriName> attributePair = std::make_pair(synonym,
                                                                   convertAttriName(attriName));
  return attributePair;
}

std::pair<std::pair<DesignEntityType, std::string>, AtrriName> Utils::ParseAttributeRef(PqlToken &token, std::unordered_map<std::string, DesignEntityType> declarations) {
  int dot_index = token.value.find('.');
  std::string synonym = token.value.substr(0, dot_index);
  std::string attriName = token.value.substr(dot_index + 1, token.value.size() - (dot_index + 1));
  std::pair<DesignEntityType, std::string> synonym_pair = std::make_pair(declarations.at(synonym), synonym);
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> attributePair = std::make_pair(synonym_pair,
                                                                                                convertAttriName(attriName));
  return attributePair;
}

AtrriName Utils::convertAttriName(std::string attriName) {
  if (attriName == "varName") {
    return AtrriName::VARNAME;
  } else if (attriName == "procName") {
    return AtrriName::PROCNAME;
  } else if (attriName == "stmt#") {
    return AtrriName::STMTNO;
  } else if (attriName == "value") {
    return AtrriName::VALUE;
  } else {
    throw "Invalid Attribute Name";
  }
}

std::string Utils::TrimQuotes(std::string& s) {
  return s.substr(1, s.size() - 2);
}

std::string Utils::TrimUnderscoreAndQuotes(std::string& s) {
  return s.substr(2, s.size() - 4);
}