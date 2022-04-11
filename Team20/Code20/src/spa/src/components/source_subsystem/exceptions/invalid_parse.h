#ifndef INVALID_PARSE_H
#define INVALID_PARSE_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class InvalidParseException : public Exception {
 public:
  [[nodiscard]] const char *what() const noexcept override = 0;
};

}

#endif //INVALID_PARSE_H
