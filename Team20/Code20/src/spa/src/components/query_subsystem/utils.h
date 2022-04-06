#ifndef UTILS_H
#define UTILS_H

#include "pql_lexer/pql_token.h"
#include <algorithm>

enum class AttriName {
  PROCNAME,
  VARNAME,
  VALUE,
  STMTNO
};

enum class DesignEntityType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE
};

class Utils {
 public:
  static std::string RemoveSpace(const std::string &token);
  static bool ContainsDot(const std::string &token);
  static AttriName convertAttriName(std::string attriName);
  static std::pair<std::pair<DesignEntityType, std::string>, AttriName> ParseAttributeRef(PqlToken &token, std::unordered_map<std::string, DesignEntityType> declarations);
  static std::pair<std::string, AttriName> ParseAttributeRef(PqlToken &token);
  static std::string TrimQuotes(std::string&);
  static std::string TrimUnderscoreAndQuotes(std::string&);
  static bool IsConversionNeeded(DesignEntityType, AttriName);
  static std::vector<std::string> SplitString(std::string&, char);
};

#endif //UTILS_H
