#ifndef UTILS_H
#define UTILS_H

#include "pql_lexer/pql_token.h"

enum class AtrriName {
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
  static AtrriName convertAttriName(std::string attriName);
  static std::pair<std::pair<DesignEntityType, std::string>, AtrriName> ParseAttributeRef(PqlToken &token, std::unordered_map<std::string, DesignEntityType> declarations);
  static std::pair<std::string, AtrriName> ParseAttributeRef(PqlToken &token);
  static std::string TrimQuotes(std::string&);
  static std::string TrimUnderscoreAndQuotes(std::string&);
  static std::unordered_set<DesignEntityType> int_ident_entities;
  static bool IsConversionNeeded(DesignEntityType, AtrriName);
};

#endif //UTILS_H
