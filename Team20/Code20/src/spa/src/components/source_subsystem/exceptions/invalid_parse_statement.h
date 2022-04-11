#ifndef INVALID_PARSE_STATEMENT_H
#define INVALID_PARSE_STATEMENT_H

#include "invalid_parse.h"

namespace source {

class InvalidParseStatementException : public InvalidParseException {
 private:
  String m_msg;

 public:
  InvalidParseStatementException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_PARSE_STATEMENT_H
