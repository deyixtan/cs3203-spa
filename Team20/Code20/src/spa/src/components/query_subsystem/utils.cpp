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

std::unordered_set<DesignEntityType> int_ident_entities = {
    DesignEntityType::READ,
    DesignEntityType::PRINT,
    DesignEntityType::CALL
};

bool Utils::IsConversionNeeded(DesignEntityType type, AtrriName attribute_name) {
  if (int_ident_entities.count(type) != 0) {
    if (attribute_name == AtrriName::PROCNAME || attribute_name == AtrriName::VARNAME) {
      return true;
    }
  }
  return false;
}

std::unordered_set<DesignEntityType> procname_entities = {
    DesignEntityType::PROCEDURE
};

std::unordered_set<DesignEntityType> varname_entities = {
  DesignEntityType::VARIABLE
};

std::unordered_set<DesignEntityType> stmtno_entities = {
  DesignEntityType::STMT,
  DesignEntityType::WHILE,
  DesignEntityType::IF,
  DesignEntityType::ASSIGN
};

std::unordered_set<DesignEntityType> value_entities = {
  DesignEntityType::CONSTANT
};

std::unordered_set<DesignEntityType> varname_and_stmtno_entities = {
    DesignEntityType::READ,
    DesignEntityType::PRINT
};

std::unordered_set<DesignEntityType> procname_and_stmtno_entities = {
    DesignEntityType::CALL
};

std::unordered_set<AtrriName> varname_and_stmtno_attrinames = {
    AtrriName::VARNAME,
    AtrriName::STMTNO
};

std::unordered_set<AtrriName> procname_and_stmtno_attrinames = {
    AtrriName::PROCNAME,
    AtrriName::STMTNO
};