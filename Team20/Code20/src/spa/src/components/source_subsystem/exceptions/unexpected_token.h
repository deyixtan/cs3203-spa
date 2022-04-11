#ifndef UNEXPECTED_TOKEN_H
#define UNEXPECTED_TOKEN_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class UnexpectedTokenException : public Exception {
 private:
  String m_msg;

 public:
  UnexpectedTokenException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //UNEXPECTED_TOKEN_H
