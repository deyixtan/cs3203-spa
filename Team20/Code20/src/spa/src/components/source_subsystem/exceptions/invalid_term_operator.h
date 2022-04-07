#ifndef INVALID_TERM_OPERATOR_H
#define INVALID_TERM_OPERATOR_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class InvalidTermOperatorException : public Exception {
 private:
  String m_msg;

 public:
  InvalidTermOperatorException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_TERM_OPERATOR_H
