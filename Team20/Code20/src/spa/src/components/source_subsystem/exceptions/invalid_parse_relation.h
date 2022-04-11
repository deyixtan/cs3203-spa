#ifndef INVALID_PARSE_RELATION_H
#define INVALID_PARSE_RELATION_H

#include "invalid_parse.h"

namespace source {

class InvalidParseRelationException : public InvalidParseException {
 private:
  String m_msg;

 public:
  InvalidParseRelationException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_PARSE_RELATION_H
