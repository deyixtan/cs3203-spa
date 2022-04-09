#ifndef INVALID_PARSE_FACTOR_H
#define INVALID_PARSE_FACTOR_H

#include "invalid_parse.h"

namespace source {

class InvalidParseFactorException : public InvalidParseException {
 private:
  String m_msg;

 public:
  InvalidParseFactorException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_PARSE_FACTOR_H
