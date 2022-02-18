#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_INVALID_PARSE_RELATION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_INVALID_PARSE_RELATION_H_

#include <exception>
#include <string>

#include "invalid_parse.h"

namespace source {

class InvalidParseRelationException : public InvalidParseException {
 private:
  std::string m_msg;

 public:
  static std::string MESSAGE;

 public:
  InvalidParseRelationException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_INVALID_PARSE_RELATION_H_
