#ifndef INVALID_PARSE_CONDITION_H
#define INVALID_PARSE_CONDITION_H

#include "invalid_parse.h"

namespace source {

class InvalidParseConditionException : public InvalidParseException {
 private:
  String m_msg;

 public:
  InvalidParseConditionException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_PARSE_CONDITION_H
