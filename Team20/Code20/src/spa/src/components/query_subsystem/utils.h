#ifndef UTILS_H
#define UTILS_H

#include "pql_lexer/pql_token.h"

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

extern std::unordered_set<AttriName> procedure_attribute_names;
extern std::unordered_set<AttriName> call_attribute_names;
extern std::unordered_set<AttriName> variable_attribute_names;
extern std::unordered_set<AttriName> read_and_print_attribute_names;
extern std::unordered_set<AttriName> constant_attribute_names;
extern std::unordered_set<AttriName> stmt_attribute_names;

#endif //UTILS_H
