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
  static bool IsConversionNeeded(DesignEntityType, AtrriName);
  static std::vector<std::string> SplitString(std::string&, char);
};

extern std::unordered_set<DesignEntityType> int_ident_entities;
extern std::unordered_set<DesignEntityType> procname_entities;
extern std::unordered_set<DesignEntityType> varname_entities;
extern std::unordered_set<DesignEntityType> stmtno_entities;
extern std::unordered_set<DesignEntityType> value_entities;
extern std::unordered_set<DesignEntityType> varname_and_stmtno_entities;
extern std::unordered_set<DesignEntityType> procname_and_stmtno_entities;
extern std::unordered_set<AtrriName> varname_and_stmtno_attrinames;
extern std::unordered_set<AtrriName> procname_and_stmtno_attrinames;

#endif //UTILS_H
