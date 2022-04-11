#ifndef INVALID_CALL_H
#define INVALID_CALL_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class InvalidCallException : public Exception {
 private:
  String m_msg;

 public:
  InvalidCallException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_CALL_H
