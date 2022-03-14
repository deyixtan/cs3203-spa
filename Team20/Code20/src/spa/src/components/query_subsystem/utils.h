#ifndef UTILS_H
#define UTILS_H

#include "pql_lexer/pql_token.h"

class Utils {
 public:
  static std::string RemoveSpace(const std::string &token);
  static bool ContainsDot(const std::string &token);
};

#endif //UTILS_H
